#include "video.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/timestamp.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#include <logc/log.h>
#include <thread>

#define TEXFRAME_QUEUE_SIZE 48
#define PACKET_QUEUE_SIZE   16

#define AV_SYNC_THRESHOLD   0.01
#define AV_NOSYNC_THRESHOLD 10.0

LibAVVideoDecoder::LibAVVideoDecoder(VideoParameters& outputParameters)
    : textureFrameQueue(TEXFRAME_QUEUE_SIZE)
    , videoPacketQueue(PACKET_QUEUE_SIZE)
    , mFmtCtx(nullptr)
    , mFrame(nullptr)
    , mRescaler(nullptr)
    , mVideoCodecCtx(nullptr)
    , mConvertedFrame(nullptr)
    , mConvertedFrameSize(0)
    , mVideoStreamIndex(-1)
    , mAudioStreamIndex(-1)
    , mOutputVideoParams(outputParameters)
    , finished(false)
{
}

int LibAVVideoDecoder::open(const char* path)
{
#ifdef DEBUG
    av_log_set_level(AV_LOG_DEBUG);
#endif

    //av_init_packet(mPacket);

    mFrame          = (AVFrame*)av_frame_alloc();
    mConvertedFrame = (AVFrame*)av_frame_alloc();

    int err;

    // Get file format
    AVFormatContext* pFormat = avformat_alloc_context();

    err = avformat_open_input(&pFormat, path, NULL, NULL);
    if (err < 0) {
        log_error("Cannot open file %s: %s", path, strerror(errno));
        avformat_free_context(pFormat);
        return err;
    }

    err = avformat_find_stream_info(pFormat, NULL);
    if (err < 0) {
        log_error("Cannot retrieve stream information %s", path);
        avformat_free_context(pFormat);
        return err;
    }

    this->mFmtCtx = pFormat;

    // Find first audio stream
    for (int i = 0; i < pFormat->nb_streams; i++) {
        if (mAudioStreamIndex == -1 && pFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            mAudioStreamIndex = i;
        }

        if (mVideoStreamIndex == -1 && pFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            mVideoStreamIndex = i;
        }

        if (mVideoStreamIndex != -1 && mAudioStreamIndex != -1) {
            break;
        }
    }

    if (mVideoStreamIndex < 0) {
        log_error("Couldn't find any audio streams in file: %s", path);
        return AVERROR_INVALIDDATA;
    }

    syncType = VideoSync::AudioSync;

    if (mAudioStreamIndex < 0) {
        syncType = VideoSync::VideoSync;
    }

    // Setup the video stream
    AVStream* pVideoStream = pFormat->streams[mVideoStreamIndex];

    // Find the decoder
    AVCodecParameters* pVideoCodecParams = pVideoStream->codecpar;
    const AVCodec*     cpVideoDecoder    = avcodec_find_decoder(pVideoCodecParams->codec_id);

    if (cpVideoDecoder == NULL) {
        log_error("Couldnt find decoder for codecID: %d", pVideoCodecParams->codec_id);
        avformat_free_context(pFormat);
        return AVERROR_DECODER_NOT_FOUND;
    }

    AVCodecContext* pVideoCodecContext = avcodec_alloc_context3(cpVideoDecoder);
    avcodec_parameters_to_context(pVideoCodecContext, pVideoCodecParams);

    err = avcodec_open2(pVideoCodecContext, cpVideoDecoder, NULL);
    if (err < 0) {
        log_error("Couldn't open video codec for file: %s", path);

        avcodec_close(pVideoCodecContext);
        avcodec_free_context(&pVideoCodecContext);
        avformat_free_context(pFormat);

        return err;
    }

    this->mVideoCodecCtx = pVideoCodecContext;
    this->timeBase       = av_q2d(pVideoStream->time_base);

    // Setup the converted frame buffer
    mConvertedFrameSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, mOutputVideoParams.width,
        mOutputVideoParams.height,
        16);
    av_image_alloc(mConvertedFrame->data, mConvertedFrame->linesize, mOutputVideoParams.width,
        mOutputVideoParams.height,
        AV_PIX_FMT_RGB24,
        1);

    // Init rescaler
    // TODO: determine optimal sampling method via output resolution
    mRescaler = sws_getContext(
        pVideoCodecContext->width,
        pVideoCodecContext->height,
        pVideoCodecContext->pix_fmt,
        mOutputVideoParams.width,
        mOutputVideoParams.height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        NULL, NULL, NULL);

    if (mRescaler == NULL) {
        log_error("Cannot initialize rescaler");
        return -1;
    }

#ifdef DEBUG
    av_dump_format(mFmtCtx, mVideoStreamIndex, path, 0);
#endif

    if (mAudioStreamIndex >= 0) {
        // Setup the audio stream
        AVStream* pAudioStream = pFormat->streams[mAudioStreamIndex];

        // Find the decoder
        AVCodecParameters* pAudioCodecParams = pAudioStream->codecpar;
        const AVCodec*     cpAudioDecoder    = avcodec_find_decoder(pAudioCodecParams->codec_id);

        if (cpAudioDecoder == NULL) {
            log_error("Couldnt find decoder for codecID: %d", pAudioCodecParams->codec_id);
            avformat_free_context(pFormat);
            return AVERROR_DECODER_NOT_FOUND;
        }

        AVCodecContext* pAudioCodecContext = avcodec_alloc_context3(cpAudioDecoder);
        avcodec_parameters_to_context(pAudioCodecContext, pAudioCodecParams);

        err = avcodec_open2(pAudioCodecContext, cpAudioDecoder, NULL);
        if (err < 0) {
            log_error("Couldn't open audio codec for file: %s", path);

            avcodec_close(pVideoCodecContext);
            avcodec_free_context(&pVideoCodecContext);
            avformat_free_context(pFormat);

            return err;
        }

        if (audioPlayer.init(pAudioCodecContext) < 0) {
            log_error("Unable to initialize audio player");
            return -1;
        }
    }

    mVideoState.clock.start();
    mVideoState.frameTimer = (double)av_gettime() / 1000000.0;
    mVideoState.lastDelay  = 40e-3;
    // Start the dispatching thread
    std::thread dispatchThread(LibAVVideoDecoder::dispatchThread, this);
    dispatchThread.detach();
    std::thread videoThread(LibAVVideoDecoder::videoDecodeThread, this);
    videoThread.detach();

    // Start the decoding threads
    std::thread audioThread(LibAVVideoDecoder::VideoAudioPlayer::audioThread, &this->audioPlayer);
    audioThread.detach();

    return 0;
}

void log_packet(const AVFormatContext* fmt_ctx, const AVPacket* pkt)
{
    AVRational* time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    log_info("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d",
        av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
        av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
        av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
        pkt->stream_index);
}

void log_frame(AVFrame* pFrame, AVCodecContext* pCodecContext)
{
    log_info(
        "Frame %c (%d) pts %d dts %d key_frame %d [coded_picture_number %d, display_picture_number %d]",
        av_get_picture_type_char(pFrame->pict_type),
        pCodecContext->frame_number,
        pFrame->pts,
        pFrame->pkt_dts,
        pFrame->key_frame,
        pFrame->coded_picture_number,
        pFrame->display_picture_number);
}
unsigned int LibAVVideoDecoder::updateFrame(MediaFrame& frame)
{
    double audioClock = audioPlayer.lastAudioFramePTS;
    log_info("%f, %f", frame.pts, audioClock);

    while (this->textureFrameQueue.empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //eturn;
    }
    uint8_t* oldData
        = frame.data;
    if (!this->textureFrameQueue.get(frame)) {
        return 0;
    }
    if (oldData != nullptr) {
        // Free allocated data from old frame
        av_free(oldData);
    }

    // double audioClock;
    double diff;
    double syncThreshold;
    double actualDelay;
    double delay = frame.pts - mVideoState.lastFramePTS;

    actualDelay = frame.pts - audioClock;

    if (actualDelay - timeBase < 10E-7) {
        actualDelay = timeBase;
    }

    log_info("Actual delay: %f", actualDelay);
    log_info("Queue size: %u", audioPlayer.audioPacketQueue.mCurrentSize.load());

    return (unsigned int)(actualDelay * 1000);
}

void LibAVVideoDecoder::dispatchThread(LibAVVideoDecoder* obj)
{
    AVPacket* packet = av_packet_alloc();

    int videoStreamIndex = obj->mVideoStreamIndex;
    int audioStreamIndex = obj->mAudioStreamIndex;

    bool audioQueueActive = true;

    SPSCRingBuffer<AVPacket*>& videoPacketQueue = obj->videoPacketQueue;
    SPSCRingBuffer<AVPacket*>& audioPacketQueue = obj->audioPlayer.audioPacketQueue;

    int ret;

    videoPacketQueue.reset();
    audioPacketQueue.reset();

    // Receive and dispatch packets
    do {
        ret = av_read_frame(obj->mFmtCtx, packet);
        if (ret == AVERROR_EOF) {
            obj->finished = true;
            av_packet_free(&packet);
            return;
        }

        if (ret < 0) {
            log_error("Dispatch thread: Error reading packet: %s", LibAVVideoDecoder::getError(ret));
            obj->finished = true;
            av_packet_free(&packet);
            return;
        }
        if (audioQueueActive && packet->stream_index == audioStreamIndex) {
            while (audioPacketQueue.full()) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }

            AVPacket* audioPacket = av_packet_clone(packet);
            //log_packet(obj->mFmtCtx, packet);

            audioPacketQueue.put(audioPacket);
            av_packet_unref(packet);
        } else if (packet->stream_index == videoStreamIndex) {
            while (videoPacketQueue.full()) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
            AVPacket* videoPacket = av_packet_clone(packet);
            videoPacketQueue.put(videoPacket);
            av_packet_unref(packet);
        } else {
            log_warn("Dispatch thread: Unknown stream id %u encountered: ", packet->stream_index);
            av_packet_unref(packet);
        }
    } while (true);
}

void LibAVVideoDecoder::videoDecodeThread(LibAVVideoDecoder* obj)
{
    int ret;
    //    double       timeBaseDouble   = ;
    const size_t inputFrameHeight = obj->mVideoCodecCtx->height;
    AVPacket*    packet;

#ifdef DEBUG
    log_debug("Video decoding thread with id %u started", std::this_thread::get_id);
#endif

    obj->textureFrameQueue.reset();

    while (true) {
        if (obj->finished) {
            return;
        }

        // Receive packet
        while (obj->videoPacketQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

        obj->videoPacketQueue.get(packet);

        ret = avcodec_send_packet(obj->mVideoCodecCtx, packet);
        if (ret < 0) {
            obj->finished = true;
            log_error("Video decoding thread: Error sending packet to decoder: %s", LibAVVideoDecoder::getError(ret));
            return;
        }
        size_t dts = packet->dts;
        av_packet_unref(packet);

        // Decode all frames from the packet
        do {

            ret = avcodec_receive_frame(obj->mVideoCodecCtx, obj->mFrame);
            if (ret == AVERROR_EOF) {
                obj->finished = true;
                break;
            }
            if (ret == AVERROR(EAGAIN)) {
                continue;
            }
            // TODO: avoid malloc and implement a custom allocator
            uint8_t* frameData = (uint8_t*)av_malloc(obj->mConvertedFrameSize);
            if (frameData == nullptr) {
                log_error("Video decoder thread: out of memory");
                break;
            }

            uint8_t* dst[4] = { frameData, nullptr, nullptr, nullptr };

            ret = sws_scale(
                obj->mRescaler, obj->mFrame->data,
                obj->mFrame->linesize, 0, inputFrameHeight,
                dst, obj->mConvertedFrame->linesize);
            if (ret < 0) {
                log_error("Video decoding thread: Error rescaling: %s", LibAVVideoDecoder::getError(ret));
                obj->finished = true;
                break;
            }

            MediaFrame frame;
            frame.data = frameData;
            frame.size = obj->mConvertedFrameSize;

            double pts = obj->mFrame->best_effort_timestamp;
            // Multiply PTS with base time unit to get the actual time value
            pts *= av_q2d(obj->mFmtCtx->streams[obj->mVideoStreamIndex]->time_base);

            // Update video state
            if (pts == 0.0) {
                pts = obj->mVideoState.videoClock;
            } else {
                obj->mVideoState.videoClock = pts;
            }

            double frameDelay = obj->mFrame->repeat_pict * av_q2d(obj->mFmtCtx->streams[obj->mVideoStreamIndex]->time_base) * 0.5;
            obj->mVideoState.videoClock += frameDelay;
            frame.pts = obj->mVideoState.videoClock;

            //obj->mVideoState.videoClock;

            while (obj->textureFrameQueue.full()) {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }

            obj->textureFrameQueue.put(frame);
        } while (ret >= 0);
    }
}

const char* LibAVVideoDecoder::getError(int errorCode)
{
    int err = av_strerror(errorCode, LibAVVideoDecoder::errorMsgBuffer, VIDEO_DECODER_ERROR_BUFFER_SIZE);
    if (err < 0) {
        return "Unknown error code";
    }

    return (const char*)LibAVVideoDecoder::errorMsgBuffer;
}

LibAVVideoDecoder::VideoAudioPlayer::VideoAudioPlayer()
    : audioPacketQueue(256)
    , mAudioCodecCtx(nullptr)
    , mResampler(nullptr)
    , mFrame(nullptr)
    , mPacket(nullptr)
    , mResampleBuffer(nullptr)
    , mResampleBufferSize(0)
    , finished(false)
{
}

int LibAVVideoDecoder::VideoAudioPlayer::init(AVCodecContext* audioCodecContext)
{
    alGenSources(1, &mSource);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio source");
        return -1;
    }
    alSourcef(mSource, AL_PITCH, 1);

    alGenBuffers(VIDEO_AUDIO_BUFFER_NO, mBuffers);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio buffer");
        return -1;
    }

    if (audioCodecContext == nullptr) {
        log_error("Error initializing video audio stream: AVCodecContext* nullptr");
        return -1;
    }
    mAudioCodecCtx = audioCodecContext;

    mFrame  = (AVFrame*)av_frame_alloc();
    mPacket = av_packet_alloc();

    mResampler = swr_alloc_set_opts(NULL,
        mAudioCodecCtx->channel_layout,
        AV_SAMPLE_FMT_S16,
        48000,
        mAudioCodecCtx->channel_layout,
        mAudioCodecCtx->sample_fmt,
        mAudioCodecCtx->sample_rate,
        0,
        nullptr);

    int init = swr_init(mResampler);

    return 0;
}
// TODO: fill buffer with silence to avoid spurious crackles
bool LibAVVideoDecoder::VideoAudioPlayer::decodeAudioFrame()
{
    ssize_t dataSize;

    if (finished) {
        return false;
    }

    int ret = avcodec_receive_frame(mAudioCodecCtx, mFrame);

    if (ret == AVERROR_EOF) {
        this->finished = true;
        return false;
    } else if (ret == AVERROR(EAGAIN)) {
        // The decoder is done with the previous packet, fetch and send a new one
        if (audioPacketQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        if (!audioPacketQueue.get(mPacket)) {
            this->finished = true;
            return false;
        }

        do {
            ret = avcodec_send_packet(mAudioCodecCtx, mPacket);

            if (ret < 0) {
                this->finished = true;
                log_error("Error sending packet to decoder");
                return false;
            }
            ret = avcodec_receive_frame(mAudioCodecCtx, mFrame);
            if (ret == AVERROR_EOF) {
                this->finished = true;
                return false;
            }

        } while (ret == AVERROR(EAGAIN));
        av_packet_unref(mPacket);

    } else if (ret < 0) {
        log_error("Video player: Audio thread: %s", LibAVVideoDecoder::getError(ret));
        return false;
    }

    if (!mResampleBuffer || mResampleBufferSize < mFrame->nb_samples) {
        av_freep(&mResampleBuffer);
        ret = av_samples_alloc(&mResampleBuffer, NULL, mFrame->channels, mFrame->nb_samples, AV_SAMPLE_FMT_S16, 1);

        if (ret < 0) {
            log_error("Video player: Audio thread: %s", LibAVVideoDecoder::getError(ret));
            return false;
        }

        mResampleBufferSize = mFrame->nb_samples;
    }

    dataSize = swr_convert(mResampler, &mResampleBuffer, mFrame->nb_samples, (const uint8_t**)mFrame->extended_data, mFrame->nb_samples);

    if (dataSize < 0) {
        return false;
    }

    lastAudioFramePTS = av_q2d(mAudioCodecCtx->time_base) * mFrame->pts - (0.09 * (VIDEO_AUDIO_BUFFER_NO - 2));

    if (lastAudioFramePTS < 0.) {
        lastAudioFramePTS = 0.0;
    }

    currentFrame.data = mResampleBuffer;
    currentFrame.size = dataSize * mFrame->channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    return true;
}

inline int LibAVVideoDecoder::VideoAudioPlayer::bufferData(uint8_t* dst, size_t dstSize)
{
    size_t  got             = 0;
    ssize_t decodedDataSize = 0;

    while (got < dstSize) {

        if (mCurrentFrameReadPos >= currentFrame.size) {

            if (!decodeAudioFrame()) {
                break;
            }
            mCurrentFrameReadPos = 0;
        }

        size_t remaining = std::min<size_t>(dstSize - got, currentFrame.size - mCurrentFrameReadPos);

        memcpy(dst, currentFrame.data + mCurrentFrameReadPos, remaining);
        mCurrentFrameReadPos += remaining;
        dst += remaining;
        got += remaining;
    }

    return got;
}

void LibAVVideoDecoder::VideoAudioPlayer::audioThread(VideoAudioPlayer* obj)
{
    ALsizei i;

    ALuint  mSource  = obj->mSource;
    ALuint* mBuffers = obj->mBuffers;

    alSourceRewind(mSource);
    alSourcei(mSource, AL_BUFFER, 0);

    bool endOfPlayback = false;

    // Initial fill
    /* Fill the buffer queue */
    for (i = 0; i < VIDEO_AUDIO_BUFFER_NO; i++) {

        size_t got = obj->bufferData(obj->decodedDataBuffer, VIDEO_AUDIO_BUFFER_SIZE);
        if (got < VIDEO_AUDIO_BUFFER_SIZE) {
            endOfPlayback = true;
        }

        alBufferData(mBuffers[i], AL_FORMAT_STEREO16, obj->decodedDataBuffer, got, 48000);
        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio for playback");
            return;
        }

        if (endOfPlayback) {
            break;
        }
    }
    alSourceQueueBuffers(mSource, i, mBuffers);
    alSourcePlay(mSource);
    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Error starting playback\n");
        return;
    }

    if (endOfPlayback) {
        obj->finished = true;
        return;
    }

    while (true) {
        ALint processed, state;

        bool endOfPlayback = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        /* Get relevant source info */
        alGetSourcei(mSource, AL_SOURCE_STATE, &state);
        alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);

        while (processed > 0) {
            ALuint bufid;
            alSourceUnqueueBuffers(mSource, 1, &bufid);

            processed--;

            size_t got = obj->bufferData(obj->decodedDataBuffer, VIDEO_AUDIO_BUFFER_SIZE);
            if (got < VIDEO_AUDIO_BUFFER_SIZE) {
                endOfPlayback = true;
            }

            alBufferData(bufid, AL_FORMAT_STEREO16, obj->decodedDataBuffer, got, 48000);
            alSourceQueueBuffers(mSource, 1, &bufid);

            if (alGetError() != AL_NO_ERROR) {
                log_error("Error buffering audio data");
                return;
            }

            if (endOfPlayback) {
                break;
            }
        }

        if (state != AL_PLAYING && state != AL_PAUSED) {

            ALint queued;

            alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
            if (queued == 0) {
                return;
            }

            alSourcePlay(mSource);
            if (alGetError() != AL_NO_ERROR) {
                return;
            }
        }
    }
    if (endOfPlayback) {
        obj->finished = true;
        return;
    }
}

void LibAVVideoDecoder::VideoAudioPlayer::close()
{
    alDeleteSources(1, &mSource);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to delete audio source");
        return;
    }

    alDeleteBuffers(VIDEO_AUDIO_BUFFER_NO, mBuffers);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to delete audio buffer");
        return;
    }
}
char LibAVVideoDecoder::errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE] = { 0 };

#include "video.hpp"
#include "av_utils.hpp"
#include "constants.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/timestamp.h>
#include <libswscale/swscale.h>
}

#include <engine.hpp>
#include <file/configuration.hpp>
#include <logc/log.h>

#include <thread>

#define TEXFRAME_QUEUE_SIZE 16
#define PACKET_QUEUE_SIZE   16

#define MIN_VIDEO_DELAY_SEC 0.01

LibAVVideoDecoder::LibAVVideoDecoder()
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
    , finished(false)
    , mDisplayConfiguration(Engine::Engine::configManager.getConfiguration(Constants::VideoDecoder::cfgDisplayConfigName))
{
    // No error checking since this is guaranteed to be initialized when the engine is started
    mOutputVideoParams.width  = mDisplayConfiguration.nGetUInt(Constants::VideoDecoder::cfgScreenWidth).value;
    mOutputVideoParams.height = mDisplayConfiguration.nGetUInt(Constants::VideoDecoder::cfgScreenHeight).value;
}

int LibAVVideoDecoder::open(const char* path)
{
#ifdef DEBUG
    av_log_set_level(AV_LOG_DEBUG);
#endif

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

        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);

        return err;
    }

    this->mFmtCtx = pFormat;

    // Find first audio stream
    for (unsigned int i = 0; i < pFormat->nb_streams; i++) {
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
        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);
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
        avformat_close_input(&pFormat);
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
        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);

        return err;
    }

    this->mVideoCodecCtx = pVideoCodecContext;
    this->timeBase       = av_q2d(pVideoStream->time_base);

    // Allocate frames
    mFrame          = (AVFrame*)av_frame_alloc();
    mConvertedFrame = (AVFrame*)av_frame_alloc();

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
    int scalingAlg = ((unsigned long)(pVideoCodecContext->width * pVideoCodecContext->height) > (mOutputVideoParams.width * mOutputVideoParams.height)) ? SWS_BICUBIC : SWS_BILINEAR;

    mRescaler = sws_getContext(
        pVideoCodecContext->width,
        pVideoCodecContext->height,
        pVideoCodecContext->pix_fmt,
        mOutputVideoParams.width,
        mOutputVideoParams.height,
        AV_PIX_FMT_RGB24,
        scalingAlg,
        NULL, NULL, NULL);

    if (mRescaler == NULL) {
        log_error("Cannot initialize rescaler");

        avcodec_close(pVideoCodecContext);
        avcodec_free_context(&pVideoCodecContext);
        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);

        av_frame_free(&mFrame);
        av_frame_free(&mConvertedFrame);

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

            sws_freeContext(mRescaler);

            avcodec_close(pVideoCodecContext);
            avcodec_free_context(&pVideoCodecContext);
            avformat_close_input(&pFormat);
            avformat_free_context(pFormat);

            av_frame_free(&mFrame);
            av_frame_free(&mConvertedFrame);

            return AVERROR_DECODER_NOT_FOUND;
        }

        AVCodecContext* pAudioCodecContext = avcodec_alloc_context3(cpAudioDecoder);
        avcodec_parameters_to_context(pAudioCodecContext, pAudioCodecParams);

        err = avcodec_open2(pAudioCodecContext, cpAudioDecoder, NULL);
        if (err < 0) {
            log_error("Couldn't open audio codec for file: %s", path);

            sws_freeContext(mRescaler);

            avcodec_close(pVideoCodecContext);
            avcodec_free_context(&pVideoCodecContext);

            avcodec_close(pAudioCodecContext);
            avcodec_free_context(&pAudioCodecContext);
            avformat_close_input(&pFormat);
            avformat_free_context(pFormat);

            av_frame_free(&mFrame);
            av_frame_free(&mConvertedFrame);

            return err;
        }

        if (audioPlayer.init(pAudioCodecContext, pAudioStream) < 0) {
            log_error("Unable to initialize audio player");

            sws_freeContext(mRescaler);

            avcodec_close(pVideoCodecContext);
            avcodec_free_context(&pVideoCodecContext);

            avcodec_close(pAudioCodecContext);
            avcodec_free_context(&pAudioCodecContext);
            avformat_close_input(&pFormat);
            avformat_free_context(pFormat);

            av_frame_free(&mFrame);
            av_frame_free(&mConvertedFrame);

            return -1;
        }
    }

    // Start the dispatching thread
    std::thread dispatchThread(LibAVVideoDecoder::dispatchThread, this);
    dispatchThread.detach();

    std::thread videoThread(LibAVVideoDecoder::videoDecodeThread, this);
    videoThread.detach();

    // Start the decoding threads
    std::thread audioThread(VideoAudioPlayer::audioThread, &this->audioPlayer);
    audioThread.detach();

    return 0;
}

void LibAVVideoDecoder::close()
{
    this->finished = true;

    audioPlayer.close();

    // Sleep a bit to make sure the all queues in threads are flushed
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    AVPacket* leftover;

    while (!videoPacketQueue.empty()) {
        videoPacketQueue.get(leftover);
        av_packet_free(&leftover);
    }

    MediaFrame leftoverTextureFrame;
    while (!textureFrameQueue.empty()) {
        textureFrameQueue.get(leftoverTextureFrame);
        av_free(leftoverTextureFrame.data);
    }

    sws_freeContext(mRescaler);

    avcodec_close(mVideoCodecCtx);
    avcodec_free_context(&mVideoCodecCtx);
    avformat_close_input(&mFmtCtx);
    avformat_free_context(mFmtCtx);

    av_frame_free(&mFrame);
    av_frame_free(&mConvertedFrame);
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

size_t LibAVVideoDecoder::updateFrame(MediaFrame& frame)
{
    double audioClock = audioPlayer.lastAudioFramePTS;

    if (this->textureFrameQueue.empty()) {
        return 0;
        // std::this_thread::sleep_for(std::chrono::milliseconds((int)(timeBase - MIN_VIDEO_DELAY_SEC)));
    }

    uint8_t* oldData = frame.data;
    if (!this->textureFrameQueue.get(frame)) {
        return 0;
    }
    if (oldData != nullptr) {
        // Free allocated data from old frame
        av_free(oldData);
    }

    double actualDelay = frame.pts - audioClock;

    if (actualDelay - (timeBase - MIN_VIDEO_DELAY_SEC) < 10E-7) {
        actualDelay = timeBase - MIN_VIDEO_DELAY_SEC; //0.023;
    }

    return (size_t)(actualDelay * 1000000);
}

void LibAVVideoDecoder::dispatchThread(LibAVVideoDecoder* obj)
{
    AVPacket* packet = av_packet_alloc();

    if (packet == nullptr) {
        log_fatal("Video decoding thread: Out of memory!");
        return;
    }

    int videoStreamIndex = obj->mVideoStreamIndex;
    int audioStreamIndex = obj->mAudioStreamIndex;

    SPSCRingBuffer<AVPacket*>& videoPacketQueue = obj->videoPacketQueue;
    SPSCRingBuffer<AVPacket*>& audioPacketQueue = obj->audioPlayer.audioPacketQueue;

    int ret;

    videoPacketQueue.reset();
    audioPacketQueue.reset();

    // Receive and dispatch packets
    do {

        if (obj->finished) {
            break;
        }

        ret = av_read_frame(obj->mFmtCtx, packet);
        if (ret == AVERROR_EOF) {
            obj->finished = true;
            av_packet_free(&packet);
            return;
        }

        if (ret < 0) {
            log_error("Dispatch thread: Error reading packet: %s", LibAV::getError(ret));
            obj->finished = true;
            av_packet_free(&packet);
            return;
        }

        if (packet->stream_index == audioStreamIndex) {
            while (audioPacketQueue.full() && !obj->finished) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }

            if (obj->finished) {
                break;
            }

            AVPacket* audioPacket = av_packet_clone(packet);
            audioPacketQueue.put(audioPacket);
            av_packet_unref(packet);
        } else if (packet->stream_index == videoStreamIndex) {
            while (videoPacketQueue.full() && !obj->finished) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }

            if (obj->finished) {
                break;
            }

            AVPacket* videoPacket = av_packet_clone(packet);
            videoPacketQueue.put(videoPacket);
            av_packet_unref(packet);
        } else {
            log_warn("Dispatch thread: Unknown stream id %u encountered: ", packet->stream_index);
            av_packet_unref(packet);
        }
    } while (true);

    av_packet_free(&packet);

    return;
}

void LibAVVideoDecoder::videoDecodeThread(LibAVVideoDecoder* obj)
{
    int          ret;
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
            log_error("Video decoding thread: Error sending packet to decoder: %s", LibAV::getError(ret));
            return;
        }
        av_packet_unref(packet);

        // Decode all frames from the packet
        do {

            if (obj->finished) {
                return;
            }

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
                log_error("Video decoding thread: Error rescaling: %s", LibAV::getError(ret));
                obj->finished = true;
                break;
            }

            MediaFrame frame;
            frame.data = frameData;
            frame.size = obj->mConvertedFrameSize;

            double pts = obj->mFrame->best_effort_timestamp;
            // Multiply PTS with base time unit to get the actual time value
            pts *= av_q2d(obj->mFmtCtx->streams[obj->mVideoStreamIndex]->time_base);

            double frameDelay = obj->mFrame->repeat_pict * av_q2d(obj->mFmtCtx->streams[obj->mVideoStreamIndex]->time_base) * 0.5;
            frame.pts         = pts - frameDelay;

            while (obj->textureFrameQueue.full()) {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }

            obj->textureFrameQueue.put(frame);
        } while (ret >= 0);
    }
}

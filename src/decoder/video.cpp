#include "video.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#include <thread>

#include <logc/log.h>

LibAVVideoDecoder::LibAVVideoDecoder()
    : mFmtCtx(nullptr)
    , mPacket(nullptr)
    , mFrame(nullptr)
    , mRescaler(nullptr)
    , mConversionBuffer(nullptr)
    , mVideoCodecCtx(nullptr)
    , mConvertedFrame(nullptr)
    , mConvertedFrameSize(0)
    , mVideoStreamIndex(-1)
    , mAudioStreamIndex(-1)
    , finished(false)
{
}

unsigned int LibAVVideoDecoder::getWidth()
{
    return mVideoCodecCtx->width;
}

unsigned int LibAVVideoDecoder::getHeight()
{
    return mVideoCodecCtx->height;
}

int LibAVVideoDecoder::open(const char* path)
{
#ifdef DEBUG
    av_log_set_level(AV_LOG_DEBUG);
#endif

    mPacket = av_packet_alloc();
    av_init_packet(mPacket);

    mFrame          = (AVFrame*)av_frame_alloc();
    mConvertedFrame = (AVFrame*)av_frame_alloc();

    int err;

    // Get audio file format
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
    int iAudioStreamIndex = -1;
    for (int i = 0; i < pFormat->nb_streams; i++) {
        if (mAudioStreamIndex == -1 && pFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            mAudioStreamIndex = i;
            break;
        }

        if (mVideoStreamIndex == -1 && pFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            mVideoStreamIndex = i;
            break;
        }

        if (mVideoStreamIndex != -1 && mAudioStreamIndex != -1) {
            break;
        }
    }

    if (mVideoStreamIndex < 0) {
        log_error("Couldn't find any audio streams in file: %s", path);
        return AVERROR_INVALIDDATA;
    }

    if (mAudioStreamIndex < 0) {
        log_warn("Couldn't find any audio streams in file: %s", path);
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
        log_error("Couldn't open codec for file: %s", path);

        avcodec_close(pVideoCodecContext);
        avcodec_free_context(&pVideoCodecContext);
        avformat_free_context(pFormat);

        return err;
    }

    this->mVideoCodecCtx = pVideoCodecContext;

    // Setup the converted frame buffer
    mConvertedFrameSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, mVideoCodecCtx->width, mVideoCodecCtx->height, 16);
    mConversionBuffer   = (uint8_t*)av_malloc(mConvertedFrameSize);
    av_image_fill_arrays(mConvertedFrame->data, mConvertedFrame->linesize, mConversionBuffer, AV_PIX_FMT_RGB24, mVideoCodecCtx->width, mVideoCodecCtx->height, 1);
    // Init rescaler
    mRescaler = sws_getContext(
        pVideoCodecContext->width,
        pVideoCodecContext->height,
        pVideoCodecContext->pix_fmt,
        pVideoCodecContext->width,
        pVideoCodecContext->height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        NULL, NULL, NULL);

    // Start the clock and the decoding thread
    mVideoState.videoClock.start();
    std::thread decodeThread(LibAVVideoDecoder::decodeThread, this);
    decodeThread.detach();

    return 0;
}

void LibAVVideoDecoder::updateFrame(VideoTextureFrame& frame)
{
    std::lock_guard<std::mutex> stateLock(mVideoState.stateMutex);

    // Do not fetch new frame until this finished
    if (mVideoState.videoClock.getElapsedSeconds() < frame.pts) {
        return;
    }

    // Free allocated data from old frame
    av_free(frame.data);
    // Fetch new frame;
    LibAVVideoDecoder::textureFrameQueue.get(frame);
}

void LibAVVideoDecoder::decodeThread(LibAVVideoDecoder* obj)
{
    int ret;

    const int    videoStreamIndex = obj->mVideoStreamIndex;
    const int    audioStreamIndex = obj->mAudioStreamIndex;
    const double timeBaseDouble   = av_q2d(obj->mVideoCodecCtx->time_base);

#ifdef DEBUG
    log_debug("Video decoding thread with id %u started", std::this_thread::get_id);
#endif

    LibAVVideoDecoder::textureFrameQueue.reset();

    while (true) {
        if (obj->finished) {
            return;
        }

        ret = avcodec_receive_frame(obj->mVideoCodecCtx, obj->mFrame);

        if (ret == AVERROR_EOF) {
            obj->finished = true;
            return;
        } else if (ret == AVERROR(EAGAIN)) {
            // The decoder is done with the previous packet, send a new one
            do {
                ret = av_read_frame(obj->mFmtCtx, obj->mPacket);
                if (ret < 0) {
                    log_error("Video decoding thread: Error reading packet: %s", LibAVVideoDecoder::getError(ret));
                    obj->finished = true;
                    return;
                }
                if (obj->mPacket->stream_index == videoStreamIndex) {
                    break;
                }
            } while (true);
            do {
                ret = avcodec_send_packet(obj->mVideoCodecCtx, obj->mPacket);
                av_packet_unref(obj->mPacket);

                if (ret < 0) {
                    obj->finished = true;
                    log_error("Video decoding thread: Error sending packet to decoder: %s", LibAVVideoDecoder::getError(ret));
                    return;
                }
                ret = avcodec_receive_frame(obj->mVideoCodecCtx, obj->mFrame);
                if (ret == AVERROR_EOF) {
                    obj->finished = true;
                    return;
                }
            } while (ret == AVERROR(EAGAIN));
        }

        ret = sws_scale(
            obj->mRescaler, obj->mFrame->data,
            obj->mFrame->linesize, 0, obj->mConvertedFrame->height,
            obj->mConvertedFrame->data, obj->mConvertedFrame->linesize);

        if (ret < 0) {
            log_error("Video decoding thread: Error rescaling: %s", LibAVVideoDecoder::getError(ret));
            obj->finished = true;
            return;
        }
        // TODO: avoid malloc and implement a custom allocator
        VideoTextureFrame frame;
        uint8_t*          frameData = (uint8_t*)av_malloc(obj->mConvertedFrameSize); //; new uint8_t[obj->mConvertedFrameSize];
        if (frameData == nullptr) {
            log_error("Video decoder thread: out of memory");
            return;
        }
        // Copy converted data to the new frame
        memcpy(frameData, obj->mConversionBuffer, obj->mConvertedFrameSize);

        frame.data = frameData;
        frame.size = obj->mConvertedFrameSize;

        double pts = (obj->mPacket->dts != AV_NOPTS_VALUE) ? obj->mFrame->best_effort_timestamp : 0.0;

        // Multiply PTS with base time unit to get the actual time value
        pts *= timeBaseDouble;

        std::lock_guard<std::mutex> stateLock(obj->mVideoState.stateMutex);
        // Update video state
        if (pts == 0.0) {
            pts = obj->mVideoState.lastFramePTS;
        } else {
            obj->mVideoState.lastFramePTS = pts;
        }

        frame.pts = pts;

        double frameDelay = obj->mFrame->repeat_pict * timeBaseDouble * 0.5;

        LibAVVideoDecoder::textureFrameQueue.put(frame);
        obj->mVideoState.lastFramePTS += frameDelay;
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

SPSCRingBuffer<VideoTextureFrame> LibAVVideoDecoder::textureFrameQueue { 500 };
char                              LibAVVideoDecoder::errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE] = { 0 };

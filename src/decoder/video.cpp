#include "video.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/timestamp.h>
#include <libswscale/swscale.h>
}

#include <thread>

#include <logc/log.h>

LibAVVideoDecoder::LibAVVideoDecoder(VideoParameters& outputParameters)
    : textureFrameQueue(128)
    , mFmtCtx(nullptr)
    , mPacket(nullptr)
    , mFrame(nullptr)
    , mRescaler(nullptr)
    , mConversionBuffer(nullptr)
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

    mPacket = av_packet_alloc();
    // av_init_packet(mPacket);

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

    // Start the decoding thread
    std::thread decodeThread(LibAVVideoDecoder::decodeThread, this);
    decodeThread.detach();

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

void LibAVVideoDecoder::updateFrame(VideoTextureFrame& frame)
{
    if (this->textureFrameQueue.empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    uint8_t* oldData = frame.data;
    if (!this->textureFrameQueue.get(frame)) {
        return;
    }

    if (oldData != nullptr) {
        // Free allocated data from old frame
        av_free(oldData);
    }
}

void LibAVVideoDecoder::dispatchThread(LibAVVideoDecoder* obj)
{
    // Receive packet
    do {
        ret = av_read_frame(obj->mFmtCtx, obj->mPacket);
        if (ret == AVERROR_EOF) {
            obj->finished = true;
            return;
        }

        if (ret < 0) {
            log_error("Video decoding thread: Error reading packet: %s", LibAVVideoDecoder::getError(ret));
            obj->finished = true;
            return;
        }
        if (obj->mPacket->stream_index == videoStreamIndex) {
            break;
        }

    } while (true);

    ret = avcodec_send_packet(obj->mVideoCodecCtx, obj->mPacket);
    if (ret < 0) {
        obj->finished = true;
        log_error("Video decoding thread: Error sending packet to decoder: %s", LibAVVideoDecoder::getError(ret));
        return;
    }
}

void LibAVVideoDecoder::decodeThread(LibAVVideoDecoder* obj)
{
    int ret;

    const int    videoStreamIndex = obj->mVideoStreamIndex;
    const int    audioStreamIndex = obj->mAudioStreamIndex;
    const double timeBaseDouble   = av_q2d(obj->mVideoCodecCtx->time_base);

    const size_t inputFrameHeight = obj->mVideoCodecCtx->height;

#ifdef DEBUG
    log_debug("Video decoding thread with id %u started", std::this_thread::get_id);
#endif

    obj->textureFrameQueue.reset();

    while (true) {
        if (obj->finished) {
            return;
        }

        // Receive packet
        do {
            ret = av_read_frame(obj->mFmtCtx, obj->mPacket);
            if (ret == AVERROR_EOF) {
                obj->finished = true;
                return;
            }

            if (ret < 0) {
                log_error("Video decoding thread: Error reading packet: %s", LibAVVideoDecoder::getError(ret));
                obj->finished = true;
                return;
            }
            if (obj->mPacket->stream_index == videoStreamIndex) {
                break;
            }

        } while (true);

        ret = avcodec_send_packet(obj->mVideoCodecCtx, obj->mPacket);
        if (ret < 0) {
            obj->finished = true;
            log_error("Video decoding thread: Error sending packet to decoder: %s", LibAVVideoDecoder::getError(ret));
            return;
        }

        // Decode all frames from the packet
        do {

            ret = avcodec_receive_frame(obj->mVideoCodecCtx, obj->mFrame);
            if (ret == AVERROR_EOF) {
                obj->finished = true;
                return;
            }
            if (ret == AVERROR(EAGAIN)) {
                continue;
            }

            // TODO: avoid malloc and implement a custom allocator
            uint8_t* frameData = (uint8_t*)av_malloc(obj->mConvertedFrameSize);
            if (frameData == nullptr) {
                log_error("Video decoder thread: out of memory");
                return;
            }

            uint8_t* dst[4] = { frameData, nullptr, nullptr, nullptr };

            ret = sws_scale(
                obj->mRescaler, obj->mFrame->data,
                obj->mFrame->linesize, 0, inputFrameHeight,
                dst, obj->mConvertedFrame->linesize);
            if (ret < 0) {
                log_error("Video decoding thread: Error rescaling: %s", LibAVVideoDecoder::getError(ret));
                obj->finished = true;
                return;
            }

            VideoTextureFrame frame;
            frame.data     = frameData;
            frame.size     = obj->mConvertedFrameSize;
            frame.linesize = obj->mConvertedFrame->linesize[0];

            double pts = (obj->mPacket->dts != AV_NOPTS_VALUE) ? obj->mFrame->best_effort_timestamp : 0.0;

            // Multiply PTS with base time unit to get the actual time value
            pts *= timeBaseDouble;

            // Update video state
            if (pts == 0.0) {
                pts = obj->mVideoState.lastFramePTS;
            } else {
                obj->mVideoState.lastFramePTS = pts;
            }

            frame.pts = pts;

            double frameDelay = obj->mFrame->repeat_pict * timeBaseDouble * 0.5;

            while (obj->textureFrameQueue.full()) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }

            obj->textureFrameQueue.put(frame);
            obj->mVideoState.lastFramePTS += frameDelay;
        } while (ret >= 0);

        av_packet_unref(obj->mPacket);
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

char LibAVVideoDecoder::errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE] = { 0 };

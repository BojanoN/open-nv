#include "video.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#include <logc/log.h>

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

    return 0;
}

int LibAVVideoDecoder::getFrame(uint8_t* dst, size_t maxDstSize)
{
    if (!decodeFrame()) {
        this->finished = true;
        return -1;
    }

    if (mConvertedFrameSize > maxDstSize) {
        log_error("Insufficient video frame buffer size, please increase the buffer size");
        return AVERROR_BUFFER_TOO_SMALL;
    }

    memcpy(dst, mConversionBuffer, mConvertedFrameSize);

    return mConvertedFrameSize;
}

bool LibAVVideoDecoder::decodeFrame()
{
    ssize_t dataSize;

    if (finished) {
        return false;
    }

    int ret = avcodec_receive_frame(mVideoCodecCtx, mFrame);

    if (ret == AVERROR_EOF) {
        this->finished = true;
        return false;
    } else if (ret == AVERROR(EAGAIN)) {
        // The decoder is done with the previous packet, send a new one
        ret = av_read_frame(mFmtCtx, mPacket);
        do {
            ret = avcodec_send_packet(mVideoCodecCtx, mPacket);
            av_packet_unref(mPacket);

            if (ret < 0) {
                this->finished = true;
                log_error("Error sending packet to decoder");
                return false;
            }
            ret = avcodec_receive_frame(mVideoCodecCtx, mFrame);
            if (ret == AVERROR_EOF) {
                this->finished = true;
                return false;
            }
        } while (ret == AVERROR(EAGAIN));
    }

    dataSize = sws_scale(
        mRescaler, mFrame->data,
        mFrame->linesize, 0, mConvertedFrame->height,
        mConvertedFrame->data, mConvertedFrame->linesize);

    if (dataSize < 0) {
        return false;
    }

    return true;
}

const char* LibAVVideoDecoder::getError(int errno)
{
    int err = av_strerror(errno, LibAVVideoDecoder::errorMsgBuffer, VIDEO_DECODER_ERROR_BUFFER_SIZE);

    if (err < 0) {
        return "Unknown error code";
    }

    return (const char*)LibAVVideoDecoder::errorMsgBuffer;
}

#include "audio.hpp"

#include <errno.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

#include <logc/log.h>
#include <util/ringbuffer.hpp>

LibAVDecoder::~LibAVDecoder()
{
}

LibAVDecoder::LibAVDecoder()
    : mCodecCtx(nullptr)
    , mFmtCtx(nullptr)
    , mPacket(nullptr)
    , mFrame(nullptr)
    , mResampler(nullptr)
    , mResampleBuffer(nullptr)
    , mResampleBufferSize(0)
    , finished(false)

{
#ifdef DEBUG
    av_log_set_level(AV_LOG_DEBUG);
#endif
}

int LibAVDecoder::open(const char* path)
{

    mPacket = av_packet_alloc();
    av_init_packet(mPacket);
    mFrame = (AVFrame*)av_frame_alloc();

    // Get audio file format
    AVFormatContext* pFormat = avformat_alloc_context();
    if (avformat_open_input(&pFormat, path, NULL, NULL) != 0) {
        log_error("Cannot open file %s: %s", path, strerror(errno));
        avformat_free_context(pFormat);
        return -1;
    }

    if (avformat_find_stream_info(pFormat, NULL) < 0) {
        log_error("Cannot retrieve stream information %s", path);
        avformat_free_context(pFormat);
        return -1;
    }

    this->mFmtCtx = pFormat;

    // Find first audio stream
    int iAudioStreamIndex = -1;
    for (int i = 0; i < pFormat->nb_streams; i++) {
        if (pFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            iAudioStreamIndex = i;
            break;
        }
    }

    if (iAudioStreamIndex < 0) {
        log_error("Couldn't find any audio streams in file: %s", path);
        avformat_free_context(pFormat);
        return -1;
    }

    AVStream* pStream = pFormat->streams[iAudioStreamIndex];

    // Find the decoder
    AVCodecParameters* pCodecParams = pStream->codecpar;
    const AVCodec*     cpDecoder    = avcodec_find_decoder(pCodecParams->codec_id);

    if (cpDecoder == NULL) {
        log_error("Couldnt find decoder for codecID: %d", pCodecParams->codec_id);
        avformat_free_context(pFormat);
        return -1;
    }

    AVCodecContext* pCodecContext = avcodec_alloc_context3(cpDecoder);
    avcodec_parameters_to_context(pCodecContext, pCodecParams);

    if (avcodec_open2(pCodecContext, cpDecoder, NULL) < 0) {
        log_error("Couldn't open codec for file: %s", path);

        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        avformat_free_context(pFormat);

        return -1;
    }

    this->mCodecCtx          = pCodecContext;
    AVSampleFormat sampleFmt = (enum AVSampleFormat)mCodecCtx->sample_fmt;

    /*
    if (sampleFmt == AV_SAMPLE_FMT_FLTP) {
        mOutSampleFormat = AV_SAMPLE_FMT_FLT;
    } else {
        mOutSampleFormat = sampleFmt;
    }*/

    mOutSampleFormat = AV_SAMPLE_FMT_S16;
    mResampler       = swr_alloc_set_opts(NULL,
        mCodecCtx->channel_layout,
        mOutSampleFormat,
        48000,
        mCodecCtx->channel_layout,
        mCodecCtx->sample_fmt,
        mCodecCtx->sample_rate,
        0,
        nullptr);

    int init = swr_init(mResampler);
    if (init < 0) {
        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        avformat_free_context(pFormat);
        log_error("Couldn't init software resampler: %s", path);

        return -1;
    }
    return 0;
}

void LibAVDecoder::close()
{
    if (this->mCodecCtx) {
        avcodec_close(mCodecCtx);
        avcodec_free_context(&mCodecCtx);
    }

    if (this->mFmtCtx) {
        avformat_free_context(mFmtCtx);
    }

    av_frame_free(&mFrame);
}

bool LibAVDecoder::frameEmpty()
{
    return currentFrameInfo.currentOffset >= currentFrameInfo.size;
}

inline int LibAVDecoder::decodeData(uint8_t* dst, size_t dstSize)
{
    size_t got = 0;

    while (got < dstSize) {

        if (frameEmpty()) {
            if (!decodeFrame()) {
                break;
            }
            currentFrameInfo.currentOffset = 0;
        }

        size_t remaining = std::min<size_t>(dstSize - got, currentFrameInfo.size - currentFrameInfo.currentOffset);

        memcpy(dst, currentFrameInfo.data[0] + currentFrameInfo.currentOffset, remaining);
        currentFrameInfo.currentOffset += remaining;
        dst += remaining;
        got += remaining;
    }

    return got;
}

bool LibAVDecoder::decodeFrame()
{
    ssize_t dataSize;

    if (finished) {
        return false;
    }

    int ret = avcodec_receive_frame(mCodecCtx, mFrame);

    if (ret == AVERROR_EOF) {
        this->finished = true;
        return false;
    } else if (ret == AVERROR(EAGAIN)) {
        // The decoder is done with the previous packet, send a new one
        int ret = av_read_frame(mFmtCtx, mPacket);
        do {
            ret = avcodec_send_packet(mCodecCtx, mPacket);
            av_packet_unref(mPacket);

            if (ret < 0) {
                this->finished = true;
                log_error("Error sending packet to decoder");
                return false;
            }
            ret = avcodec_receive_frame(mCodecCtx, mFrame);
            if (ret == AVERROR_EOF) {
                this->finished = true;
                return false;
            }
        } while (ret == AVERROR(EAGAIN));
    }

    if (!mResampleBuffer || mResampleBufferSize < mFrame->nb_samples) {
        av_freep(&mResampleBuffer);
        if (av_samples_alloc(&mResampleBuffer, NULL, mFrame->channels, mFrame->nb_samples, mOutSampleFormat, 1)
            < 0) {
            return false;
        }
    }
    dataSize = swr_convert(mResampler, &mResampleBuffer, mFrame->nb_samples,
        (const uint8_t**)mFrame->extended_data, mFrame->nb_samples);

    if (dataSize < 0) {
        return false;
    }
    currentFrameInfo.data = &mResampleBuffer;
    currentFrameInfo.size = dataSize * mFrame->channels * av_get_bytes_per_sample((enum AVSampleFormat)mOutSampleFormat);

    return true;
}
unsigned int LibAVDecoder::getSampleRate()
{
    return mFrame->sample_rate;
}

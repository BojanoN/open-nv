#include "audio.hpp"

#include <errno.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}

#include <logc/log.h>
#include <util/ringbuffer.hpp>

AudioDecoder::~AudioDecoder()
{
}

AudioDecoder::AudioDecoder()
    : mCodecCtx(nullptr)
    , mFmtCtx(nullptr)
    , mPacket(nullptr)
    , mFrame(nullptr)
    , finished(false)

{
    av_log_set_level(AV_LOG_DEBUG);
}

int AudioDecoder::openFile(const char* path)
{

    mPacket = av_packet_alloc();
    av_init_packet(mPacket);
    avcodec_register_all();
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

    this->mCodecCtx = pCodecContext;

    return 0;
}

void AudioDecoder::close()
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

bool AudioDecoder::frameEmpty()
{
    return currentFrameInfo.currentOffset >= currentFrameInfo.size;
}

int AudioDecoder::decodeData(uint8_t* dst, size_t dstSize)
{
    size_t got = 0;

    while (got < dstSize) {

        if (frameEmpty()) {
            if (!decodeFrame()) {
                break;
            }
            currentFrameInfo.currentOffset = 0;
            currentFrameInfo.size          = mFrame->nb_samples * av_get_channel_layout_nb_channels(mFrame->channel_layout) * av_get_bytes_per_sample((enum AVSampleFormat)mFrame->format);
            currentFrameInfo.data          = mFrame->data;
        }

        size_t remaining = std::min<size_t>(dstSize - got, currentFrameInfo.size - currentFrameInfo.currentOffset);

        memcpy(dst, currentFrameInfo.data[0] + currentFrameInfo.currentOffset, remaining);
        currentFrameInfo.currentOffset += remaining;
        got += remaining;
    }

    return got;
}

bool AudioDecoder::decodeFrame()
{

    if (finished) {
        return false;
    }

    int ret = avcodec_receive_frame(mCodecCtx, mFrame);

    if (ret == AVERROR_EOF) {
        this->finished = true;
        return false;
    } else if (ret == AVERROR(EAGAIN)) {

        int ret = av_read_frame(mFmtCtx, mPacket);

        ret = avcodec_send_packet(mCodecCtx, mPacket);
        av_packet_unref(mPacket);

        if (ret < 0) {
            this->finished = true;
            log_error("Error sending packet to decoder");
            return false;
        }
    }

    return true;
}

unsigned int AudioDecoder::getSampleRate()
{
    return mFrame->sample_rate;
}

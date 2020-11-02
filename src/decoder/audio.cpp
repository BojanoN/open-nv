#include "audio.hpp"

#include <errno.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

#include "av_utils.hpp"
#include <engine/audio.hpp>
#include <logc/log.h>
#include <util/ringbuffer.hpp>

#include <thread>
#include <unordered_map>

#define DEFAULT_AUDIO_SAMPLE_FMT AV_SAMPLE_FMT_S16
#define MESSAGE_QUEUE_SIZE       64

inline int openContext(LibAVAudioContext* ctx)
{
    const char* path = ctx->path;
    int         err;
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

        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);
        return err;
    }

    // Find first audio stream
    int iAudioStreamIndex = -1;
    for (unsigned int i = 0; i < pFormat->nb_streams; i++) {
        if (pFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            iAudioStreamIndex = i;
            break;
        }
    }

    if (iAudioStreamIndex < 0) {
        log_error("Couldn't find any audio streams in file: %s", path);

        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);
        return AVERROR_INVALIDDATA;
    }

    AVStream* pStream = pFormat->streams[iAudioStreamIndex];

    // Find the decoder
    AVCodecParameters* pCodecParams = pStream->codecpar;
    const AVCodec*     cpDecoder    = avcodec_find_decoder(pCodecParams->codec_id);

    if (cpDecoder == NULL) {
        log_error("Couldnt find decoder for codecID: %d", pCodecParams->codec_id);

        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);
        return AVERROR_DECODER_NOT_FOUND;
    }

    AVCodecContext* pCodecContext = avcodec_alloc_context3(cpDecoder);
    avcodec_parameters_to_context(pCodecContext, pCodecParams);

    err = avcodec_open2(pCodecContext, cpDecoder, NULL);
    if (err < 0) {
        log_error("Couldn't open codec for file: %s", path);

        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);

        return err;
    }

    ctx->mCodecCtx = pCodecContext;
    ctx->mFmtCtx   = pFormat;

    unsigned int deviceSampleRate = AudioEngine::getCurrentDevice().sampleRate;

    ctx->mResampler = swr_alloc_set_opts(NULL,
        pCodecContext->channel_layout,
        DEFAULT_AUDIO_SAMPLE_FMT,
        deviceSampleRate,
        pCodecContext->channel_layout,
        pCodecContext->sample_fmt,
        pCodecContext->sample_rate,
        0,
        nullptr);

    err = swr_init(ctx->mResampler);
    if (err < 0) {
        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormat);
        avformat_free_context(pFormat);

        log_error("Couldn't initialize software resampler: %s", path);

        return err;
    }

    ctx->mPacket = av_packet_alloc();
    ctx->mFrame  = (AVFrame*)av_frame_alloc();
    av_init_packet(ctx->mPacket);

    ctx->finished.store(false, std::memory_order_relaxed);

    return 0;
}

inline void closeContext(LibAVAudioContext* ctx)
{

    av_frame_free(&ctx->mFrame);
    av_packet_free(&ctx->mPacket);

    avcodec_close(ctx->mCodecCtx);
    avcodec_free_context(&ctx->mCodecCtx);
    avformat_close_input(&ctx->mFmtCtx);
    avformat_free_context(ctx->mFmtCtx);

    swr_free(&ctx->mResampler);
}

inline int decodeFrame(LibAVAudioContext* ctx)
{
    ssize_t dataSize;

    if (ctx->isFinished()) {
        return -1;
    }

    AVPacket* pPacket = ctx->mPacket;
    AVFrame*  pFrame  = ctx->mFrame;

    int ret = avcodec_receive_frame(ctx->mCodecCtx, pFrame);

    if (ret == AVERROR_EOF) {
        return false;
    } else if (ret == AVERROR(EAGAIN)) {
        // The decoder is done with the previous packet, send a new one
        int ret = av_read_frame(ctx->mFmtCtx, ctx->mPacket);
        do {
            ret = avcodec_send_packet(ctx->mCodecCtx, pPacket);
            av_packet_unref(pPacket);

            if (ret < 0) {
                log_error("Error sending packet to decoder");
                return ret;
            }
            ret = avcodec_receive_frame(ctx->mCodecCtx, pFrame);
            if (ret == AVERROR_EOF) {
                return ret;
            }
        } while (ret == AVERROR(EAGAIN));
    }

    if (!ctx->mResampleBuffer || (int)ctx->mResampleBufferSize < pFrame->nb_samples) {

        av_freep(&ctx->mResampleBuffer);
        ret = av_samples_alloc(&ctx->mResampleBuffer, NULL, pFrame->channels, pFrame->nb_samples, DEFAULT_AUDIO_SAMPLE_FMT, 1);

        if (ret < 0) {
            return ret;
        }

        ctx->mResampleBufferSize = pFrame->nb_samples;
    }

    dataSize = swr_convert(ctx->mResampler, &ctx->mResampleBuffer, pFrame->nb_samples, (const uint8_t**)pFrame->extended_data, pFrame->nb_samples);
    if (dataSize < 0) {
        return ret;
    }

    ctx->mResampledFrame.size = dataSize * pFrame->channels * av_get_bytes_per_sample((enum AVSampleFormat)DEFAULT_AUDIO_SAMPLE_FMT);
    ctx->mResampledFrame.data = ctx->mResampleBuffer;

    return 0;
}

inline int decodeData(LibAVAudioContext* ctx, size_t dstSize)
{
    size_t     got          = 0;
    FrameInfo& currentFrame = ctx->mResampledFrame;
    int        err;

    AudioFrame* dstFrame = ctx->deviceBuffer->peek();
    if (dstFrame == nullptr) {
        return -1;
    }

    uint8_t* dstBuffer = dstFrame->data;

    while (got < dstSize) {
        if (currentFrame.currentOffset >= currentFrame.size) {
            err = decodeFrame(ctx);
            if (err < 0) {
                ctx->finished.store(true, std::memory_order_acq_rel);
                break;
            }
            currentFrame.currentOffset = 0;
        }

        size_t remaining = std::min<size_t>(dstSize - got, currentFrame.size - currentFrame.currentOffset);

        memcpy(dstBuffer, currentFrame.data + currentFrame.currentOffset, remaining);
        dstBuffer += remaining;
        currentFrame.currentOffset += remaining;
        got += remaining;
    }

    ctx->deviceBuffer->incTail();

    return got;
}

void initialBufferFill(LibAVAudioContext* ctx)
{
    for (unsigned int i = 0; i < ctx->deviceBuffer->capacity; i++) {
        decodeData(ctx, DEFAULT_DECODER_BUF_SIZE);
    }
}

void LibAVDecoder::decodeThread()
{

#ifdef DEBUG
    av_log_set_level(AV_LOG_DEBUG);
#endif

    // TODO: pool allocator for LibAVAudioContext structs
    std::unordered_map<unsigned int, LibAVAudioContext*> currentAudioContexts {};
    DecoderMessage                                       currentMessage;

    log_info("Audio decoding thread: starting...");

    while (true) {
        while (messageQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        messageQueue.get(currentMessage);
        switch (currentMessage.messageType) {
        case DecoderMessageType::FrameRequest: {
            auto entry = currentAudioContexts.find(currentMessage.messageData.contextID);
            if (entry == currentAudioContexts.end()) {
                // ignore bogus requests
                break;
            }
            LibAVAudioContext* ctx = entry->second;

            decodeData(ctx, DEFAULT_DECODER_BUF_SIZE);
            if (ctx->isFinished()) {
                currentAudioContexts.erase(ctx->ID);
                closeContext(ctx);
            }
            break;
        }
        case DecoderMessageType::QueueAudioContext: {
            // TODO: cache files in memory to avoid IO overhead
            LibAVAudioContext* ctx = currentMessage.messageData.audioContext;

            int err = openContext(ctx);
            if (err < 0) {
                log_error("Audio decoding thread: Unable to open audio context %u: %s", ctx->ID, LibAV::getError(err));
                break;
            }

            // Fill whole buffer
            initialBufferFill(ctx);
            if (ctx->isFinished()) {
                currentAudioContexts.erase(ctx->ID);
                closeContext(ctx);
                break;
            }
            currentAudioContexts[ctx->ID] = ctx;

            break;
        }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

void LibAVDecoder::init()
{
    std::thread decodingThread(decodeThread);
    decodingThread.detach();
}

SPSCRingBuffer<DecoderMessage> LibAVDecoder::messageQueue { MESSAGE_QUEUE_SIZE };

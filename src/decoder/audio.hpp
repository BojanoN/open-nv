#pragma once

#include <atomic>
#include <cstdint>
#include <util/ringbuffer.hpp>

#include "structs.hpp"

extern "C" {
#include <libavformat/avformat.h>
}

#define DEFAULT_DECODER_BUF_SIZE 8096

struct SwrContext;

struct FrameInfo {
    size_t   currentOffset = 0;
    size_t   size          = 0;
    uint8_t* data          = nullptr;
};

struct AudioFrame {
    uint8_t data[DEFAULT_DECODER_BUF_SIZE];
};

struct LibAVAudioContext {
    bool isFinished() { return this->finished.load(std::memory_order_relaxed); };
    bool isReady() { return this->ready.load(std::memory_order_relaxed); };

    AVCodecContext*             mCodecCtx           = nullptr;
    AVFormatContext*            mFmtCtx             = nullptr;
    AVPacket*                   mPacket             = nullptr;
    AVFrame*                    mFrame              = nullptr;
    SwrContext*                 mResampler          = nullptr;
    uint8_t*                    mResampleBuffer     = nullptr;
    size_t                      mResampleBufferSize = 0;
    FrameInfo                   mResampledFrame;
    SPSCRingBuffer<AudioFrame>* deviceBuffer = nullptr;
    std::atomic<bool>           finished     = false;
    std::atomic<bool>           ready        = false;
    unsigned int                ID           = 0;

    const char* path;
};

enum class DecoderMessageType {
    QueueAudioContext,
    FrameRequest
};

struct DecoderMessage {
    DecoderMessageType messageType;
    union {
        unsigned int       contextID;
        LibAVAudioContext* audioContext;
    } messageData;
};

class LibAVDecoder {
public:
    void                           init();
    SPSCRingBuffer<DecoderMessage> messageQueue { MESSAGE_QUEUE_SIZE };

private:
    void decodeThread();

    inline constexpr static size_t MESSAGE_QUEUE_SIZE = 64;
};

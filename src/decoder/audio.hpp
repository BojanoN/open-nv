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

    AVCodecContext*             mCodecCtx;
    AVFormatContext*            mFmtCtx;
    AVPacket*                   mPacket;
    AVFrame*                    mFrame;
    SwrContext*                 mResampler;
    uint8_t*                    mResampleBuffer;
    size_t                      mResampleBufferSize;
    FrameInfo                   mResampledFrame;
    SPSCRingBuffer<AudioFrame>* deviceBuffer;
    std::atomic<bool>           finished;
    unsigned int                ID;

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
    static void decodeThread();

    static SPSCRingBuffer<DecoderMessage> messageQueue;
};

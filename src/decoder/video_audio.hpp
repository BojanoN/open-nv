#pragma once

#include <cstddef>
#include <cstdint>

#include <util/ringbuffer.hpp>

#include "structs.hpp"

extern "C" {
#include <AL/al.h>
}

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVStream;
struct AVFrame;
struct SwrContext;

#define VIDEO_AUDIO_BUFFER_NO   8
#define VIDEO_AUDIO_BUFFER_SIZE 8192

class VideoAudioPlayer {
public:
    VideoAudioPlayer();
    int                       init(AVCodecContext* audioCodecContext, AVStream* stream);
    void                      close();
    static void               audioThread(VideoAudioPlayer* objptr);
    SPSCRingBuffer<AVPacket*> audioPacketQueue;
    double                    lastAudioFramePTS;

private:
    bool decodeAudioFrame();
    int  bufferData(uint8_t* dst, size_t dstSize);

    AVCodecContext* mAudioCodecCtx;
    SwrContext*     mResampler;
    AVFrame*        mFrame;
    AVPacket*       mPacket;

    uint8_t* mResampleBuffer;
    size_t   mResampleBufferSize;

    uint8_t decodedDataBuffer[VIDEO_AUDIO_BUFFER_SIZE] = { 0 };

    ALuint mSource;
    ALuint mBuffers[VIDEO_AUDIO_BUFFER_NO];

    AVStream* stream;

    MediaFrame currentFrame;
    size_t     mCurrentFrameReadPos;

    std::atomic<bool> finished;
    std::atomic<bool> audioThreadFinished;
};

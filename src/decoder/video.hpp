#pragma once

#include <cstddef>
#include <cstdint>
#include <mutex>

#include <util/ringbuffer.hpp>
#include <util/timer.hpp>

#define VIDEO_DECODER_ERROR_BUFFER_SIZE 1024

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;

struct VideoTextureFrame {
    uint8_t* data = nullptr;
    size_t   size = 0;
    double   pts  = 0.0;
};

// TODO: add mutex for inter-thread clock syncing
// Video player class should only handle drawing

struct VideoState {
    double            lastFramePTS;
    VideoTextureFrame currentTextureFrame;
    Timer             videoClock;
    std::mutex        stateMutex;
};

class LibAVVideoDecoder {
public:
    int  open(const char* path);
    void updateFrame(VideoTextureFrame& dst);
    bool isFinished() { return finished; }

    unsigned int getHeight();
    unsigned int getWidth();

    LibAVVideoDecoder();

    static void        decodeThread(LibAVVideoDecoder* objptr);
    static const char* getError(int errorCode);

    VideoState mVideoState;

private:
    AVFormatContext* mFmtCtx;

    AVPacket* mPacket;
    AVFrame*  mFrame;

    SwsContext* mRescaler;
    uint8_t*    mConversionBuffer;

    AVCodecContext* mVideoCodecCtx;
    AVFrame*        mConvertedFrame;
    size_t          mConvertedFrameSize;

    int mVideoStreamIndex;
    int mAudioStreamIndex;

    bool finished;

    static SPSCRingBuffer<VideoTextureFrame> textureFrameQueue;
    static char                              errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE];
};

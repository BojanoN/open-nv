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
    uint8_t* data     = nullptr;
    size_t   size     = 0;
    double   pts      = 0.0;
    size_t   linesize = 0;
};
struct VideoState {
    double lastFramePTS;
};

struct VideoParameters {
    size_t width;
    size_t height;

    VideoParameters(size_t w, size_t h)
        : width(w)
        , height(h)
    {
    }
};

class LibAVVideoDecoder {
public:
    int  open(const char* path);
    void updateFrame(VideoTextureFrame& dst);
    bool isFinished() { return finished; }

    unsigned int getHeight() { return mOutputVideoParams.height; };
    unsigned int getWidth() { return mOutputVideoParams.width; };

    LibAVVideoDecoder(VideoParameters& outputVideoPameters);

    static void        decodeThread(LibAVVideoDecoder* objptr);
    static const char* getError(int errorCode);

    VideoState                        mVideoState;
    SPSCRingBuffer<VideoTextureFrame> textureFrameQueue;

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

    VideoParameters mOutputVideoParams;

    bool finished;

    static char errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE];
};

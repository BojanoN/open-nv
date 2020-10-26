#pragma once

#include <cstddef>
#include <cstdint>

#include <util/ringbuffer.hpp>

#include "structs.hpp"
#include "video_audio.hpp"

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVStream;
struct AVFrame;
struct SwsContext;
struct SwrContext;

struct VideoState {
    double lastFramePTS;
    double videoClock;
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

enum class VideoSync {
    AudioSync,
    VideoSync,
    ExternalSync
};

#define VIDEO_AUDIO_BUFFER_NO   8
#define VIDEO_AUDIO_BUFFER_SIZE 8192

class LibAVVideoDecoder {
public:
    int  open(const char* path);
    void close();

    void updateFrame(MediaFrame& dst);
    bool isFinished() { return finished; }

    unsigned int getHeight() { return mOutputVideoParams.height; };
    unsigned int getWidth() { return mOutputVideoParams.width; };

    LibAVVideoDecoder(size_t width, size_t height);

    static void videoDecodeThread(LibAVVideoDecoder* objptr);
    static void dispatchThread(LibAVVideoDecoder* obj);

    VideoState mVideoState;

    SPSCRingBuffer<MediaFrame> textureFrameQueue;
    SPSCRingBuffer<AVPacket*>  videoPacketQueue;

private:
    VideoAudioPlayer audioPlayer;

    VideoSync        syncType;
    AVFormatContext* mFmtCtx;
    AVFrame*         mFrame;
    SwsContext*      mRescaler;
    AVCodecContext*  mVideoCodecCtx;
    AVFrame*         mConvertedFrame;
    size_t           mConvertedFrameSize;

    int mVideoStreamIndex;
    int mAudioStreamIndex;

    VideoParameters mOutputVideoParams;

    double timeBase;

    bool finished;
};

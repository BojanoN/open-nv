#pragma once

#include <cstddef>
#include <cstdint>

#include <util/ringbuffer.hpp>
#include <util/timer.hpp>

#include "structs.hpp"
#include "video_audio.hpp"

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVStream;
struct AVFrame;
struct SwsContext;
struct SwrContext;
namespace File {
class Configuration;

}

struct VideoState {
    double lastFramePTS;
    double videoClock;
};

struct VideoParameters {
    size_t width  = 0;
    size_t height = 0;
};

enum class VideoSync {
    AudioSync,
    VideoSync,
    ExternalSync
};

#define VIDEO_AUDIO_BUFFER_NO   8
#define VIDEO_AUDIO_BUFFER_SIZE 8192

using File::Configuration;

class LibAVVideoDecoder {

    


public:
    int  open(const char* path);
    void close();

    size_t updateFrame(MediaFrame& dst);
    bool   isFinished() { return finished; }

    unsigned int getHeight() { return mOutputVideoParams.height; };
    unsigned int getWidth() { return mOutputVideoParams.width; };

    LibAVVideoDecoder();

    static void videoDecodeThread(LibAVVideoDecoder* objptr);
    static void dispatchThread(LibAVVideoDecoder* obj);

    VideoState mVideoState;

    SPSCRingBuffer<MediaFrame> textureFrameQueue;
    SPSCRingBuffer<AVPacket*>  videoPacketQueue;

    VideoParameters mOutputVideoParams;

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

    double timeBase;
    bool   finished;

    // TODO: adjust configuration and config manager so we can mark this as const
    const Configuration *const mDisplayConfiguration;
};

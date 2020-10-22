#pragma once

#include <cstddef>
#include <cstdint>
#include <mutex>

#include <util/ringbuffer.hpp>
#include <util/timer.hpp>

extern "C" {
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
}

#define VIDEO_DECODER_ERROR_BUFFER_SIZE 1024

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;
struct SwrContext;

struct MediaFrame {
    uint8_t* data = nullptr;
    size_t   size = 0;
    double   pts  = 0.0;
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

enum class VideoSync {
    AudioSync,
    VideoSync,
    ExternalSync
};

#define VIDEO_AUDIO_BUFFER_NO   8
#define VIDEO_AUDIO_BUFFER_SIZE 8192

class LibAVVideoDecoder {
public:
    int open(const char* path);
    int close() { return 42; }; // TODO:

    void updateFrame(MediaFrame& dst);
    bool isFinished() { return finished; }

    unsigned int getHeight() { return mOutputVideoParams.height; };
    unsigned int getWidth() { return mOutputVideoParams.width; };

    LibAVVideoDecoder(VideoParameters& outputVideoPameters);

    static void        videoDecodeThread(LibAVVideoDecoder* objptr);
    static void        dispatchThread(LibAVVideoDecoder* obj);
    static const char* getError(int errorCode);

    VideoState                 mVideoState;
    SPSCRingBuffer<MediaFrame> textureFrameQueue;

    SPSCRingBuffer<AVPacket*> videoPacketQueue;

private:
    class VideoAudioPlayer {
    public:
        VideoAudioPlayer();
        int                       init(AVCodecContext* audioCodecContext);
        void                      close();
        static void               audioThread(VideoAudioPlayer* objptr);
        SPSCRingBuffer<AVPacket*> audioPacketQueue;

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

        MediaFrame currentFrame;
        size_t     mCurrentFrameReadPos;

        bool finished;
    };

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

    bool finished;

    static char errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE];
};

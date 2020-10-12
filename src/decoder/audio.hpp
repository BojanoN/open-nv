#pragma once

#include <cstdint>
extern "C" {
#include <libavformat/avformat.h>
}

struct SwrContext;

struct FrameInfo {
    size_t    currentOffset;
    size_t    size;
    uint8_t** data;
};

enum class AudioSampleFormat {
    UInt8,
    Int16,
    Float32
};

class AudioDecoder {

public:
    virtual int          open(const char* path)                   = 0;
    virtual void         close()                                  = 0;
    virtual bool         decodeFrame()                            = 0;
    virtual int          decodeData(uint8_t* dst, size_t dstSize) = 0;
    virtual unsigned int getSampleRate()                          = 0;

    ~AudioDecoder() {};
    AudioDecoder() {};
};

class LibAVDecoder : public AudioDecoder {
public:
    ~LibAVDecoder();
    LibAVDecoder();

    int               open(const char* path);
    void              close();
    bool              decodeFrame();
    int               decodeData(uint8_t* dst, size_t dstSize);
    unsigned int      getSampleRate();
    AudioSampleFormat getSampleFormat();

private:
    bool frameEmpty();

    AVCodecContext*     mCodecCtx;
    AVFormatContext*    mFmtCtx;
    AVPacket*           mPacket;
    AVFrame*            mFrame;
    SwrContext*         mResampler;
    uint8_t*            mResampleBuffer;
    size_t              mResampleBufferSize;
    enum AVSampleFormat mOutSampleFormat;
    bool                finished;
    FrameInfo           currentFrameInfo;
};

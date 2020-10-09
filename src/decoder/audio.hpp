#pragma once

#include <cstdint>
#include <queue>

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVFrame;

class SPSCRingBuffer;

struct FrameInfo {
    size_t    currentOffset;
    size_t    size;
    uint8_t** data;
};

class AudioDecoder {

public:
    int  openFile(const char* path);
    void close();
    bool decodeFrame();
    int  decodeData(uint8_t* dst, size_t dstSize);

    ~AudioDecoder();
    AudioDecoder();

    unsigned int getSampleRate();
    bool         hasMore() const { return !finished; }

private:
    bool frameEmpty();

    AVCodecContext*  mCodecCtx;
    AVFormatContext* mFmtCtx;
    AVPacket*        mPacket;
    AVFrame*         mFrame;
    bool             finished;
    FrameInfo        currentFrameInfo;
};

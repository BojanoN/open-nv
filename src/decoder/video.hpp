#pragma once

#include <cstddef>
#include <cstdint>

#define VIDEO_DECODER_ERROR_BUFFER_SIZE 1024

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;

class LibAVVideoDecoder {
public:
    int open(const char* path);
    int getFrame(uint8_t* dst, size_t maxDstSize);

    unsigned int getHeight();
    unsigned int getWidth();

    static const char* getError(int errno);

private:
    bool decodeFrame();

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

    static char* errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE];
};

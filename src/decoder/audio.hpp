#pragma once

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVFrame;

class SPSCRingBuffer;

struct AudioInfo {
    unsigned int noChannels;
    unsigned int sampleRate;
};

struct AudioFrame {
    void*        data;
    unsigned int size;
};

class AudioDecoder {

public:
    int openFile(const char* path);
    // Consider adding resampling to this function, unsigned int noChannels, unsigned int sampleRate
    int decodeFrame();

    ~AudioDecoder();
    AudioDecoder();

    unsigned int getNoChannels();
    unsigned int getSampleRate();
    AVFrame*     getFrame() { return mFrame; }
    bool         isDone() { return finished; }

private:
    AVCodecContext*  mCodecCtx;
    AVFormatContext* mFmtCtx;
    AVPacket*        mPacket;
    AVFrame*         mFrame;
    int              finished;
};

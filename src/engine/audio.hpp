#pragma once

extern "C" {
#include <AL/al.h>
#include <AL/alc.h>
}

#include <alloc/pool.hpp>
#include <decoder/audio.hpp>
#include <util/ringbuffer.hpp>

#define DEFAULT_DECODER_BUF_SIZE 440000
#define POOL_ALLOC_SIZE          16

class StreamPlayer {
public:
    StreamPlayer()
        : source(0)
        , buffer(0)
        , decoderRingBuffer(DEFAULT_DECODER_BUF_SIZE) {};
    ~StreamPlayer() {};

    int openFile(const char* path);

    static ALsizei AL_APIENTRY bufferCallbackStatic(void* objptr, void* data, ALsizei size);

    ALsizei bufferCallback(void* data, ALsizei size);

    bool update();

private:
    ALuint source;
    ALuint buffer;

    SPSCRingBuffer decoderRingBuffer;
    AudioDecoder   decoder;
};

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();

    int                  playFile(const char* path);
    static AudioDecoder* newAudioDecoder();

private:
    ALCdevice*  currentDevice;
    ALCcontext* currentDeviceContext;

    static Allocator::Pool<StreamPlayer> streamPlayerAllocator;
    static Allocator::Pool<AudioDecoder> audioDecoderAllocator;
};

#pragma once

extern "C" {
#include <AL/al.h>
#include <AL/alc.h>
}

#include <util/ringbuffer.hpp>

class StreamPlayer {
public:
    StreamPlayer()
        : source(0)
        , buffer(0) {};
    ~StreamPlayer();

    int openFile(const char* path);

    static ALsizei AL_APIENTRY bufferCallbackStatic(void* objptr, void* data, ALsizei size);

    ALsizei bufferCallback(void* data, ALsizei size);

private:
    ALuint source;
    ALuint buffer;

    SPSCRingBuffer* buffer;
};

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();
    int playFile(const char* path);

private:
    ALCdevice*  currentDevice;
    ALCcontext* currentDeviceContext;
};

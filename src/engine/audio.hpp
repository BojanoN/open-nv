#pragma once

extern "C" {
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
}

#include <alloc/pool.hpp>
#include <decoder/audio.hpp>

#define DEFAULT_DECODER_BUF_SIZE 8096
#define POOL_ALLOC_SIZE          16
#define NO_BUFFERS               16

class StreamPlayer {
public:
    StreamPlayer()
        : mSource(0)
        , mBuffer(0)
        , active(false) {};
    ~StreamPlayer() {};

    int openFile(const char* path);

    static ALsizei AL_APIENTRY bufferCallbackStatic(void* objptr, void* data, ALsizei size);

    ALsizei bufferCallback(void* data, ALsizei size);

    bool update();
    bool initCallback();
    bool start();
    void close();

private:
    uint8_t      decodedDataBuffer[DEFAULT_DECODER_BUF_SIZE] = { 0 };
    ALuint       mSource;
    ALuint       mBuffer;
    ALuint       mBuffers[NO_BUFFERS];
    LibAVDecoder decoder;
    bool         active;
};

class AudioEngine {
public:
    static bool init();
    static void close();

    static StreamPlayer* playFile(const char* path);

    AudioEngine(AudioEngine& other) = delete;
    void operator=(const AudioEngine&) = delete;

    static void freeStreamPlayer(StreamPlayer* player);

    static AudioEngine* getInstance();

private:
    static ALCdevice*  currentDevice;
    static ALCcontext* currentDeviceContext;

    static bool initialized;

    static Allocator::Pool<StreamPlayer> streamPlayerAllocator;
};

#pragma once

extern "C" {
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
}

#include <alloc/pool.hpp>
#include <decoder/audio.hpp>
#include <util/ringbuffer.hpp>

#define POOL_ALLOC_SIZE 16
#define NO_BUFFERS      8

struct AudioDevice {
    ALCdevice*  device        = nullptr;
    ALCcontext* deviceContext = nullptr;

    std::string  deviceName;
    unsigned int sampleRate;
    ALenum       outputFormat;
    unsigned int bufferSize;

    bool active = false;
};

class AudioEngine;

class StreamPlayer {
public:
    StreamPlayer()
        : mSource(0)
        , deviceBuffer(NO_BUFFERS * 2)
        , active(false) {};
    ~StreamPlayer() {};

    ALuint                     mSource;
    ALuint                     mBuffers[NO_BUFFERS];
    const int                  noBuffers = NO_BUFFERS;
    SPSCRingBuffer<AudioFrame> deviceBuffer;
    LibAVAudioContext          audioContext;
    bool                       active;
};

class AudioEngine {
public:
    static bool init();
    static void close();

    static StreamPlayer*      playFile(const char* path);
    static const AudioDevice& getCurrentDevice();

    static void playingThread();

    AudioEngine(AudioEngine& other) = delete;
    void operator=(const AudioEngine&) = delete;

    static AudioEngine* getInstance();

private:
    static AudioDevice currentDevice;

    static SPSCRingBuffer<StreamPlayer*> playThreadQueue;
};

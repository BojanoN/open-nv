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
    bool init();
    void close();

    StreamPlayer*             playFile(const char* path);
    static const AudioDevice& getCurrentDevice();

    void playingThread();

    AudioEngine(AudioEngine& other) = delete;
    void operator=(const AudioEngine&) = delete;

private:
    static AudioDevice             currentDevice;
    inline constexpr static size_t PLAY_QUEUE_SIZE = 64;
    RingBuffer<StreamPlayer*>      playThreadQueue { PLAY_QUEUE_SIZE };

    LibAVDecoder mAudioDecoder;
};

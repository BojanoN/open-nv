#include "audio.hpp"

#include <cstddef>
#include <logc/log.h>
#include <stdexcept>

AudioEngine::AudioEngine()
    : currentDevice(nullptr)
    , currentDeviceContext(nullptr)
{
    const char* defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    this->currentDevice           = alcOpenDevice(defaultDeviceName);

    if (!this->currentDevice) {
        log_error("Unable to open default device: %s", defaultDeviceName);
        throw std::runtime_error { "Failed to initialize OpenAL" };
    }

    this->currentDeviceContext = alcCreateContext(this->currentDevice, NULL);
    if (!alcMakeContextCurrent(this->currentDeviceContext)) {
        log_error("Unable to make context for default device");
        throw std::runtime_error { "Failed to initialize OpenAL" };
    }
};

AudioEngine::~AudioEngine()
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(this->currentDeviceContext);
    alcCloseDevice(this->currentDevice);
}

int AudioEngine::playFile(const char* path)
{

    return 0;
}

int StreamPlayer::openFile(const char* path)
{
    alGenSources(1, &this->source);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio source");
        return -1;
    }

    alGenBuffers(1, &this->buffer);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio buffer");
        return -1;
    }

    return 0;
}

int StreamPlayer::bufferCallback(void* data, ALsizei size)
{
    size_t got = 0;

    while (got < size) {
        got += this->decoderRingBuffer.readArr(size - got, (uint8_t*)data);
    }

    return got;
}

ALsizei AL_APIENTRY bufferCallbackStatic(void* objptr, void* data, ALsizei size)
{
    return static_cast<StreamPlayer*>(objptr)->bufferCallback(data, size);
}

AudioDecoder* AudioEngine::newAudioDecoder()
{
    return AudioEngine::audioDecoderAllocator.alloc();
};

Allocator::Pool<AudioDecoder> AudioEngine::audioDecoderAllocator(POOL_ALLOC_SIZE);
Allocator::Pool<StreamPlayer> AudioEngine::streamPlayerAllocator(POOL_ALLOC_SIZE);

bool StreamPlayer::update()
{
}

#include "audio.hpp"

#include <cstddef>
#include <logc/log.h>
#include <stdexcept>

extern "C" {
#include <libavcodec/avcodec.h>
}

bool AudioEngine::init()
{
    if (initialized) {
        return false;
    }

    const char* defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    currentDevice                 = alcOpenDevice(defaultDeviceName);

    if (!currentDevice) {
        log_error("Unable to open default device: %s", defaultDeviceName);
        return true;
    }

    currentDeviceContext = alcCreateContext(currentDevice, NULL);
    if (!alcMakeContextCurrent(currentDeviceContext)) {
        log_error("Unable to make context for default device");
        return true;
    }

    initialized = true;

    return false;
}

void AudioEngine::close()
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(currentDeviceContext);
    alcCloseDevice(currentDevice);
}

StreamPlayer* AudioEngine::playFile(const char* path)
{
    StreamPlayer* ret = streamPlayerAllocator.alloc();

    if (ret == nullptr) {
        return nullptr;
    }

    if (ret->openFile(path) < 0) {
        freeStreamPlayer(ret);
        return nullptr;
    }

    return ret;
}

int StreamPlayer::openFile(const char* path)
{
    alGenSources(1, &mSource);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio source");
        return -1;
    }

    alGenBuffers(1, &mBuffer);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio buffer");
        return -1;
    }

    alGenBuffers(4, mBuffers);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio buffer");
        return -1;
    }

    if (decoder.openFile(path) < 0) {
        return -1;
    }

    active = true;

    return 0;
}

Allocator::Pool<StreamPlayer> AudioEngine::streamPlayerAllocator(POOL_ALLOC_SIZE);

bool StreamPlayer::update()
{
    ALint processed, state;

    bool endOfPlayback = false;

    /* Get relevant source info */
    alGetSourcei(mSource, AL_SOURCE_STATE, &state);
    alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);

    // Decode frame and write to the buffer
    while (processed > 0) {
        ALuint bufid;

        alSourceUnqueueBuffers(mSource, 1, &bufid);
        processed--;

        size_t got = decoder.decodeData(decodedDataBuffer, DEFAULT_DECODER_BUF_SIZE);
        if (got < DEFAULT_DECODER_BUF_SIZE) {
            endOfPlayback = true;
        }

        alBufferData(bufid, AL_FORMAT_STEREO_FLOAT32, decodedDataBuffer, got, decoder.getSampleRate());
        alSourceQueueBuffers(mSource, 1, &bufid);

        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio data");
            return false;
        }

        if (endOfPlayback) {
            break;
        }
    }

    if (state != AL_PLAYING && state != AL_PAUSED) {

        ALint queued;

        alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0) {
            return true;
        }

        alSourcePlay(mSource);
        if (alGetError() != AL_NO_ERROR) {
            return true;
        }
    }

    return endOfPlayback;
}

bool StreamPlayer::start()
{
    ALsizei i;

    alSourceRewind(mSource);
    alSourcei(mSource, AL_BUFFER, 0);

    bool endOfPlayback = false;

    /* Fill the buffer queue */
    for (i = 0; i < 4; i++) {

        size_t got = decoder.decodeData(decodedDataBuffer, DEFAULT_DECODER_BUF_SIZE);
        if (got < DEFAULT_DECODER_BUF_SIZE) {
            endOfPlayback = true;
        }

        alBufferData(mBuffers[i], AL_FORMAT_STEREO_FLOAT32, decodedDataBuffer, got, decoder.getSampleRate());
        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio for playback");
            return true;
        }

        if (endOfPlayback) {
            break;
        }
    }

    alSourceQueueBuffers(mSource, i, mBuffers);
    alSourcePlay(mSource);
    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Error starting playback\n");
        return true;
    }

    return endOfPlayback;
}

void StreamPlayer::close()
{
    if (active) {
        alDeleteSources(1, &mSource);
        alDeleteBuffers(4, mBuffers);
        active = false;

        decoder.close();
    }
}

void AudioEngine::freeStreamPlayer(StreamPlayer* player)
{
    player->close();
    return AudioEngine::streamPlayerAllocator.free(player);
}
bool        AudioEngine::initialized          = false;
ALCdevice*  AudioEngine::currentDevice        = nullptr;
ALCcontext* AudioEngine::currentDeviceContext = nullptr;

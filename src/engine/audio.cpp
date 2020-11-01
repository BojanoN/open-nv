#include "audio.hpp"

#include <cstddef>
#include <logc/log.h>
#include <stdexcept>
#include <thread>
#include <unordered_set>

extern "C" {
#include <libavcodec/avcodec.h>
}

#define PLAY_QUEUE_SIZE 64

bool AudioEngine::init()
{
    if (currentDevice.device) {
        return false;
    }

    const char* defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    ALCdevice*  defaultDevice     = alcOpenDevice(defaultDeviceName);

    if (!defaultDevice) {
        log_error("Unable to open default device: %s", defaultDeviceName);
        return true;
    }

    ALCcontext* defaultDeviceContext = alcCreateContext(defaultDevice, NULL);
    if (!alcMakeContextCurrent(defaultDeviceContext)) {
        log_error("Unable to make context for default device");
        return true;
    }

    ALCint defaultDeviceSampleRate;
    alcGetIntegerv(defaultDevice, ALC_FREQUENCY, 1, &defaultDeviceSampleRate);

    currentDevice.device        = defaultDevice;
    currentDevice.deviceContext = defaultDeviceContext;
    currentDevice.deviceName    = defaultDeviceName;
    currentDevice.sampleRate    = defaultDeviceSampleRate;
    // TODO: determine optimal buffer size
    currentDevice.bufferSize = DEFAULT_DECODER_BUF_SIZE;
    // TODO: query current format from settings
    currentDevice.outputFormat = AL_FORMAT_STEREO16;
    currentDevice.active       = true;

    // TODO: spawn playing thread

    return false;
}

inline int updateStream(StreamPlayer* stream)
{
    ALint processed, streamState;

    const AudioDevice& currentDevice = AudioEngine::getCurrentDevice();

    DecoderMessage frameRequest;
    frameRequest.messageType           = DecoderMessageType::FrameRequest;
    frameRequest.messageData.contextID = stream->audioContext.ID;

    AudioFrame* framePtr;

    /* Get relevant source info */
    alGetSourcei(stream->mSource, AL_SOURCE_STATE, &streamState);
    alGetSourcei(stream->mSource, AL_BUFFERS_PROCESSED, &processed);

    while (processed > 0) {
        ALuint bufid;
        alSourceUnqueueBuffers(stream->mSource, 1, &bufid);

        processed--;

        framePtr = stream->deviceBuffer.peek();
        if (framePtr == nullptr) {
            LibAVDecoder::messageQueue.put(frameRequest);
            continue;
        }

        alBufferData(bufid, currentDevice.outputFormat, framePtr, DEFAULT_DECODER_BUF_SIZE, currentDevice.sampleRate);
        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio data");
            return -1;
        }
        alSourceQueueBuffers(stream->mSource, 1, &bufid);

        stream->deviceBuffer.pop();
        LibAVDecoder::messageQueue.put(frameRequest);
    }

    if (streamState != AL_PLAYING && streamState != AL_PAUSED) {
        alSourcePlay(stream->mSource);
        if (alGetError() != AL_NO_ERROR) {
            return -1;
        }
    }

    return 0;
}

inline int startStream(StreamPlayer* stream)
{
    alGenSources(1, &stream->mSource);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio source");
        return -1;
    }
    alSourcef(stream->mSource, AL_PITCH, 1);

    alGenBuffers(NO_BUFFERS, stream->mBuffers);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio buffer");
        return -1;
    }

    ALsizei i;

    alSourceRewind(stream->mSource);
    alSourcei(stream->mSource, AL_BUFFER, 0);

    const AudioDevice& currentDevice = AudioEngine::getCurrentDevice();

    DecoderMessage frameRequest;
    frameRequest.messageType           = DecoderMessageType::FrameRequest;
    frameRequest.messageData.contextID = stream->audioContext.ID;

    AudioFrame* framePtr;

    /* Fill the buffer queue */
    for (i = 0; i < NO_BUFFERS; i++) {
        ALuint bufid = stream->mBuffers[i];

        framePtr = stream->deviceBuffer.peek();
        if (framePtr == nullptr) {
            LibAVDecoder::messageQueue.put(frameRequest);
            continue;
        }

        alBufferData(bufid, currentDevice.outputFormat, framePtr, DEFAULT_DECODER_BUF_SIZE, currentDevice.sampleRate);
        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio for playback");
            return -1;
        }
        alSourceQueueBuffers(stream->mSource, 1, &bufid);

        stream->deviceBuffer.pop();
        LibAVDecoder::messageQueue.put(frameRequest);

        if (stream->audioContext.isFinished()) {
            break;
        }
    }

    alSourceQueueBuffers(stream->mSource, i, stream->mBuffers);
    alSourcePlay(stream->mSource);
    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Error starting playback\n");
        return -1;
    }
    if (!stream->audioContext.isFinished()) {
        stream->active = true;
    }

    return 0;
}

void AudioEngine::playingThread()
{
    std::unordered_set<StreamPlayer*> activeStreams {};

    StreamPlayer* tmpPtr;
    unsigned int  currentID = 0;

    DecoderMessage message;

    while (true) {

        while (playThreadQueue.empty() && activeStreams.empty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        while (!playThreadQueue.empty()) {
            playThreadQueue.get(tmpPtr);
            activeStreams.insert(tmpPtr);

            tmpPtr->audioContext.ID = currentID++;

            message.messageType = DecoderMessageType::QueueAudioContext;
            // TODO: consider separating the context from the stream using a pointer
            message.messageData.audioContext = &tmpPtr->audioContext;
            LibAVDecoder::messageQueue.put(message);
        }

        for (StreamPlayer* stream : activeStreams) {
            if (!stream->active) {
                // TODO: initial buffer fill
                startStream(stream);
                continue;
            }
            updateStream(stream);
        }
    }
}
void AudioEngine::close()
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(currentDevice.deviceContext);
    alcCloseDevice(currentDevice.device);
    currentDevice.active = false;
}

StreamPlayer* AudioEngine::playFile(const char* path)
{
    StreamPlayer* ret = new StreamPlayer();
    if (ret == nullptr) {
        return nullptr;
    }

    ret->audioContext.path = path;

    if (!playThreadQueue.put(ret)) {
        log_warn("Audio playing queue is full!");
        delete ret;
        return nullptr;
    }

    return ret;
}

const AudioDevice& AudioEngine::getCurrentDevice() { return AudioEngine::currentDevice; };

AudioDevice AudioEngine::currentDevice {};

SPSCRingBuffer<StreamPlayer*> AudioEngine::playThreadQueue { PLAY_QUEUE_SIZE };

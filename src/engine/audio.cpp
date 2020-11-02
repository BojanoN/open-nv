#include "audio.hpp"

#include <cstddef>
#include <logc/log.h>
#include <stdexcept>
#include <thread>
#include <unordered_set>
#include <vector>

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

    LibAVDecoder::init();

    std::thread playThread(playingThread);
    playThread.detach();

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

        alBufferData(bufid, currentDevice.outputFormat, framePtr->data, DEFAULT_DECODER_BUF_SIZE, currentDevice.sampleRate);
        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio data");
            stream->active = false;
            return -1;
        }
        alSourceQueueBuffers(stream->mSource, 1, &bufid);

        stream->deviceBuffer.pop();
        LibAVDecoder::messageQueue.put(frameRequest);
    }

    if (streamState != AL_PLAYING && streamState != AL_PAUSED) {

        ALint queued;

        alGetSourcei(stream->mSource, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0 && stream->audioContext.isFinished()) {
            stream->active = false;
            return -1;
        }

        alSourcePlay(stream->mSource);
        if (alGetError() != AL_NO_ERROR) {
            stream->active = false;
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

        framePtr = stream->deviceBuffer.peek();
        if (framePtr == nullptr) {
            LibAVDecoder::messageQueue.put(frameRequest);
            continue;
        }

        alBufferData(stream->mBuffers[i], currentDevice.outputFormat, framePtr->data, DEFAULT_DECODER_BUF_SIZE, currentDevice.sampleRate);
        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio for playback");
            return -1;
        }

        stream->deviceBuffer.pop();
        LibAVDecoder::messageQueue.put(frameRequest);

        if (stream->audioContext.isFinished()) {
            break;
        }
    }

    alSourceQueueBuffers(stream->mSource, i, stream->mBuffers);
    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Error starting playback\n");
        return -1;
    }
    alSourcePlay(stream->mSource);
    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Error starting playback\n");
        return -1;
    }

    stream->active = true;

    return 0;
}

inline void closeStream(StreamPlayer* stream)
{

    alDeleteSources(1, &stream->mSource);
    alDeleteBuffers(NO_BUFFERS, stream->mBuffers);

    stream->active = false;
}

void AudioEngine::playingThread()
{
    std::unordered_set<StreamPlayer*> activeStreams {};
    std::vector<StreamPlayer*>        deletionQueue {};

    StreamPlayer* tmpPtr;
    unsigned int  currentID = 0;

    DecoderMessage message;

    log_info("Audio playing thread: starting...");

    while (true) {

        while (playThreadQueue.empty() && activeStreams.empty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        while (!playThreadQueue.empty()) {
            playThreadQueue.get(tmpPtr);
            activeStreams.insert(tmpPtr);
            // tmpPtr->active = true;

            tmpPtr->audioContext.ID = currentID++;

            message.messageType = DecoderMessageType::QueueAudioContext;
            // TODO: consider separating the context from the stream using a pointer
            message.messageData.audioContext = &tmpPtr->audioContext;

            tmpPtr->audioContext.deviceBuffer = &tmpPtr->deviceBuffer;

            LibAVDecoder::messageQueue.put(message);
        }

        for (StreamPlayer* stream : activeStreams) {
            if (!stream->active) {
                // TODO: initial buffer fill
                if (startStream(stream) < 0) {
                    deletionQueue.push_back(stream);
                    closeStream(stream);
                    continue;
                }
                continue;
            }

            if (updateStream(stream) < 0) {
                deletionQueue.push_back(stream);
                closeStream(stream);
                continue;
            }

            if (!stream->active) {
                deletionQueue.push_back(stream);
                closeStream(stream);
            }
        }
        if (!deletionQueue.empty()) {
            for (StreamPlayer* stream : deletionQueue) {
                activeStreams.erase(stream);
                delete stream;
            }

            deletionQueue.clear();
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
    //ret->active            = true;

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

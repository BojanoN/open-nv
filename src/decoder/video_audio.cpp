#include "video_audio.hpp"
#include "av_utils.hpp"

#include <thread>

extern "C" {
#include <AL/alc.h>
#include <AL/alext.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#define BUFFER_MSEC_LENGTH   0.09
#define AUDIO_PKT_QUEUE_SIZE 64

VideoAudioPlayer::VideoAudioPlayer()
    : audioPacketQueue(AUDIO_PKT_QUEUE_SIZE)
    , mAudioCodecCtx(nullptr)
    , mResampler(nullptr)
    , mFrame(nullptr)
    , mPacket(nullptr)
    , mResampleBuffer(nullptr)
    , mResampleBufferSize(0)
    , finished(false)
{
}

int VideoAudioPlayer::init(AVCodecContext* audioCodecContext, AVStream* stream)
{
    alGenSources(1, &mSource);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio source");
        return -1;
    }
    alSourcef(mSource, AL_PITCH, 1);

    alGenBuffers(VIDEO_AUDIO_BUFFER_NO, mBuffers);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio buffer");
        return -1;
    }

    if (audioCodecContext == nullptr) {
        log_error("Error initializing video audio stream: AVCodecContext* nullptr");
        return -1;
    }
    mAudioCodecCtx = audioCodecContext;

    mFrame  = (AVFrame*)av_frame_alloc();
    mPacket = nullptr;

    this->stream = stream;

    mResampler = swr_alloc_set_opts(NULL,
        mAudioCodecCtx->channel_layout,
        AV_SAMPLE_FMT_S16,
        48000,
        mAudioCodecCtx->channel_layout,
        mAudioCodecCtx->sample_fmt,
        mAudioCodecCtx->sample_rate,
        0,
        nullptr);

    int init = swr_init(mResampler);

    return init;
}
// TODO: fill buffer with silence to avoid spurious crackles
bool VideoAudioPlayer::decodeAudioFrame()
{
    ssize_t dataSize;

    if (finished) {
        return false;
    }

    int ret = avcodec_receive_frame(mAudioCodecCtx, mFrame);

    if (ret == AVERROR_EOF) {
        this->finished = true;
        return false;
    } else if (ret == AVERROR(EAGAIN)) {
        // The decoder is done with the previous packet, fetch and send a new one
        if (mPacket != nullptr) {
            av_packet_unref(mPacket);
        }
        if (audioPacketQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        if (!audioPacketQueue.get(mPacket)) {
            this->finished = true;
            return false;
        }

        do {
            ret = avcodec_send_packet(mAudioCodecCtx, mPacket);

            if (ret < 0) {
                this->finished = true;
                log_error("Error sending packet to decoder");
                return false;
            }
            ret = avcodec_receive_frame(mAudioCodecCtx, mFrame);
            if (ret == AVERROR_EOF) {
                this->finished = true;
                return false;
            }

        } while (ret == AVERROR(EAGAIN));
        av_packet_unref(mPacket);

    } else if (ret < 0) {
        log_error("Video player: Audio thread: %s", LibAV::getError(ret));
        return false;
    }
    // log_frame(mFrame, mAudioCodecCtx);
    if (!mResampleBuffer || (int)mResampleBufferSize < mFrame->nb_samples) {
        av_freep(&mResampleBuffer);
        ret = av_samples_alloc(&mResampleBuffer, NULL, mFrame->channels, mFrame->nb_samples, AV_SAMPLE_FMT_S16, 1);

        if (ret < 0) {
            log_error("Video player: Audio thread: %s", LibAV::getError(ret));
            return false;
        }

        mResampleBufferSize = mFrame->nb_samples;
    }

    dataSize = swr_convert(mResampler, &mResampleBuffer, mFrame->nb_samples, (const uint8_t**)mFrame->extended_data, mFrame->nb_samples);

    if (dataSize < 0) {
        return false;
    }
    lastAudioFramePTS = av_q2d(stream->time_base) * mFrame->pts - (BUFFER_MSEC_LENGTH * (VIDEO_AUDIO_BUFFER_NO));
    av_packet_free(&mPacket);

    if (lastAudioFramePTS < 0.) {
        lastAudioFramePTS = 0.0;
    }

    currentFrame.data = mResampleBuffer;
    currentFrame.size = dataSize * mFrame->channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    return true;
}

inline int VideoAudioPlayer::bufferData(uint8_t* dst, size_t dstSize)
{
    size_t got = 0;

    while (got < dstSize) {

        if (mCurrentFrameReadPos >= currentFrame.size) {

            if (!decodeAudioFrame()) {
                break;
            }
            mCurrentFrameReadPos = 0;
        }

        size_t remaining = std::min<size_t>(dstSize - got, currentFrame.size - mCurrentFrameReadPos);

        memcpy(dst, currentFrame.data + mCurrentFrameReadPos, remaining);
        mCurrentFrameReadPos += remaining;
        dst += remaining;
        got += remaining;
    }

    return got;
}

void VideoAudioPlayer::audioThread(VideoAudioPlayer* obj)
{
    ALsizei i;

    ALuint  mSource  = obj->mSource;
    ALuint* mBuffers = obj->mBuffers;

    alSourceRewind(mSource);
    alSourcei(mSource, AL_BUFFER, 0);

    bool endOfPlayback = false;

    // Initial fill
    /* Fill the buffer queue */
    for (i = 0; i < VIDEO_AUDIO_BUFFER_NO; i++) {

        size_t got = obj->bufferData(obj->decodedDataBuffer, VIDEO_AUDIO_BUFFER_SIZE);
        if (got < VIDEO_AUDIO_BUFFER_SIZE) {
            endOfPlayback = true;
        }

        alBufferData(mBuffers[i], AL_FORMAT_STEREO16, obj->decodedDataBuffer, got, 48000);
        if (alGetError() != AL_NO_ERROR) {
            log_error("Error buffering audio for playback");
            obj->audioThreadFinished.store(true, std::memory_order_acq_rel);

            return;
        }

        if (endOfPlayback) {
            obj->audioThreadFinished.store(true, std::memory_order_acq_rel);

            break;
        }
    }
    alSourceQueueBuffers(mSource, i, mBuffers);
    alSourcePlay(mSource);
    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Error starting playback\n");
        obj->audioThreadFinished.store(true, std::memory_order_acq_rel);

        return;
    }

    if (endOfPlayback) {
        obj->finished.store(true, std::memory_order_acq_rel);
        obj->audioThreadFinished.store(true, std::memory_order_acq_rel);

        return;
    }

    while (true) {
        ALint processed, state;

        bool endOfPlayback = false;

        if (obj->finished.load(std::memory_order_acq_rel)) {
            obj->audioThreadFinished.store(true, std::memory_order_acq_rel);
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(4));

        /* Get relevant source info */
        alGetSourcei(mSource, AL_SOURCE_STATE, &state);
        alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);

        while (processed > 0) {
            ALuint bufid;
            alSourceUnqueueBuffers(mSource, 1, &bufid);

            processed--;

            size_t got = obj->bufferData(obj->decodedDataBuffer, VIDEO_AUDIO_BUFFER_SIZE);
            if (got < VIDEO_AUDIO_BUFFER_SIZE) {
                endOfPlayback = true;
            }

            alBufferData(bufid, AL_FORMAT_STEREO16, obj->decodedDataBuffer, got, 48000);
            alSourceQueueBuffers(mSource, 1, &bufid);

            if (alGetError() != AL_NO_ERROR) {
                log_error("Error buffering audio data");
                obj->audioThreadFinished.store(true, std::memory_order_acq_rel);
                return;
            }

            if (endOfPlayback) {
                break;
            }
        }

        if (state != AL_PLAYING && state != AL_PAUSED) {

            ALint queued;

            alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
            if (queued == 0) {
                obj->audioThreadFinished.store(true, std::memory_order_acq_rel);
                return;
            }

            alSourcePlay(mSource);
            if (alGetError() != AL_NO_ERROR) {
                obj->audioThreadFinished.store(true, std::memory_order_acq_rel);
                return;
            }
        }
    }
    if (endOfPlayback) {
        obj->finished.store(true, std::memory_order_acq_rel);
        obj->audioThreadFinished.store(true, std::memory_order_acq_rel);
        return;
    }
}

void VideoAudioPlayer::close()
{
    this->finished.store(true, std::memory_order_acq_rel);

    while (!this->audioThreadFinished.load(std::memory_order_acq_rel)) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    alDeleteSources(1, &mSource);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to delete audio source");
        return;
    }

    alDeleteBuffers(VIDEO_AUDIO_BUFFER_NO, mBuffers);
    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to delete audio buffer");
        return;
    }

    AVPacket* leftover;

    while (!audioPacketQueue.empty()) {
        audioPacketQueue.get(leftover);
        av_packet_free(&leftover);
    }

    av_frame_free(&mFrame);
    av_free(mResampleBuffer);
    swr_free(&mResampler);
    avcodec_close(mAudioCodecCtx);
    avcodec_free_context(&mAudioCodecCtx);
}

#include "audio.hpp"

#include <errno.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}

#include <logc/log.h>

int AudioDecoder::decodeFile(const char* path, const int sample_rate, double** data, int* size)
{
    // Get audio file format
    AVFormatContext* pFormat = avformat_alloc_context();
    if (avformat_open_input(&pFormat, path, NULL, NULL) != 0) {
        log_error("Cannot open file %s: %s", path, strerror(errno));
        avformat_free_context(pFormat);
        return -1;
    }

    if (avformat_find_stream_info(pFormat, NULL) < 0) {
        log_error("Cannot retrieve stream information %s", path);
        avformat_free_context(pFormat);
        return -1;
    }

    // Find first audio stream
    int iAudioStreamIndex = -1;
    for (int i = 0; i < pFormat->nb_streams; i++) {
        if (pFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            iAudioStreamIndex = i;
            break;
        }
    }

    if (iAudioStreamIndex < 0) {
        log_error("Couldn't find any audio streams in file: %s", path);
        avformat_free_context(pFormat);
        return -1;
    }

    AVStream* pStream = pFormat->streams[iAudioStreamIndex];

    // Find the decoder
    AVCodecParameters* pCodecParams = pStream->codecpar;
    const AVCodec*     cpDecoder    = avcodec_find_decoder(pCodecParams->codec_id);

    if (cpDecoder == NULL) {
        log_error("Couldnt find decoder for codecID: %d", pCodecParams->codec_id);
        avformat_free_context(pFormat);
        return -1;
    }

    AVCodecContext* pCodecContext = avcodec_alloc_context3(cpDecoder);
    avcodec_parameters_to_context(pCodecContext, pCodecParams);

    if (avcodec_open2(pCodecContext, cpDecoder, NULL) < 0) {
        log_error("Couldn't open codec for file: %s", path);

        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        avformat_free_context(pFormat);

        return -1;
    }

    avcodec_close(pCodecContext);
    avcodec_free_context(&pCodecContext);
    avformat_free_context(pFormat);

    return 0;
}

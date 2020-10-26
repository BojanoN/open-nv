#include "av_utils.hpp"

#include <logc/log.h>

extern "C" {
#include <libavutil/error.h>
}

const char* LibAV::getError(int errorCode)
{
    int err = av_strerror(errorCode, LibAV::errorMsgBuffer, VIDEO_DECODER_ERROR_BUFFER_SIZE);
    if (err < 0) {
        return "Unknown error code";
    }

    return (const char*)LibAV::errorMsgBuffer;
}

char LibAV::errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE] = { 0 };

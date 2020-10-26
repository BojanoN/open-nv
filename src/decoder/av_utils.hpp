#pragma once

#define VIDEO_DECODER_ERROR_BUFFER_SIZE 1024

class LibAV {
public:
    static const char* getError(int errorCode);

private:
    static char errorMsgBuffer[VIDEO_DECODER_ERROR_BUFFER_SIZE];
};

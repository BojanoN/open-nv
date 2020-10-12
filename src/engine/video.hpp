#pragma once

#include <decoder/video.hpp>
#include <util/shader.hpp>

class VideoPlayer {
public:
    VideoPlayer();

    int open(const char* path);

private:
    Shader            videoShader;
    LibAVVideoDecoder decoder;

    static float        vertices[16];
    static unsigned int elements[6];
};

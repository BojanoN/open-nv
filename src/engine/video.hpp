#pragma once

#include <decoder/video.hpp>
#include <util/shader.hpp>

class VideoPlayer {
public:
    VideoPlayer();

    int  open(const char* path);
    bool update();

    static void scheduleVideoRefresh(unsigned int milliseconds);

private:
    Shader          videoShader;
    VideoParameters outputVideoParams;

    LibAVVideoDecoder decoder;

    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
    GLuint mVideoFrameTexture;

    MediaFrame mCurrentTextureFrame;

    static float        vertices[16];
    static unsigned int elements[6];
};

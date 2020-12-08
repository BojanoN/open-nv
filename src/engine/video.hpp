#pragma once

#include <decoder/video.hpp>
#include <util/shader.hpp>

#include <memory>

class VideoPlayer {
public:
    VideoPlayer(unsigned int displayWidth, unsigned int displayHeight);

    int  play(const char* path);
    void close();

    bool update();

private:
    std::shared_ptr<Shader> videoShader;
    VideoParameters         outputVideoParams;

    LibAVVideoDecoder decoder;

    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
    GLuint mVideoFrameTexture;

    MediaFrame          mCurrentTextureFrame;
    static float        vertices[16];
    static unsigned int elements[6];

    static const std::string videoVertexShader;
    static const std::string videoFragmentShader;
};

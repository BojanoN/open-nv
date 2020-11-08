#pragma once

#include <decoder/video.hpp>
#include <util/shader.hpp>

#include <memory>

struct SDL_Window;

class VideoPlayer {
public:
    VideoPlayer();

    int  play(const char* path, SDL_Window* window);
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

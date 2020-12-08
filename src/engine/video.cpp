#include "video.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

#include <resources/shader.hpp>
#include <types/errorpair.hpp>

// clang-format off
float VideoPlayer::vertices[16] = {
 -1.0f,  1.0f,  0.0f, 0.0f,
 -1.0f, -1.0f,  0.0f, 1.0f,
  1.0f, -1.0f,  1.0f, 1.0f,
  1.0f,  1.0f,  1.0f, 0.0f
};

unsigned int VideoPlayer::elements[6] = {
     0, 1, 2,
     0, 2, 3
};
// clang-format on

VideoPlayer::VideoPlayer(unsigned int width, unsigned int height)
    : decoder()
{
    Types::ErrorPair<std::shared_ptr<Shader>> err = ShaderManager::getShader(VideoPlayer::videoVertexShader, VideoPlayer::videoFragmentShader);
    if (err.fail()) {
        throw std::runtime_error("Unable to get video player shaders!");
    }

    videoShader = err.value;
}

ssize_t VideoPlayer::play(const char* path, SDL_Window* window)
{
    glEnable(GL_TEXTURE_2D);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VideoPlayer::vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int err = decoder.open(path);
    if (err < 0) {
        log_error("Unable to open video file for decoding");
        glDeleteBuffers(1, &mVBO);
        glDeleteBuffers(1, &mEBO);
        glDeleteVertexArrays(1, &mVAO);
        return -1;
    }

    glGenTextures(1, &mVideoFrameTexture);
    glBindTexture(GL_TEXTURE_2D, mVideoFrameTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outputVideoParams.width, outputVideoParams.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return this->update();

    /*
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    SDL_GL_SwapWindow(window);

                    this->close();
                    return 0;
                }
            }
        }
        if (!this->update()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SDL_GL_SwapWindow(window);

            this->close();
            return -1;
        }
        SDL_GL_SwapWindow(window);
    }*/
};

void VideoPlayer::close()
{

    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);

    glDeleteTextures(1, &mVideoFrameTexture);

    decoder.close();
    delete[] mCurrentTextureFrame.data;
}

ssize_t VideoPlayer::update()
{

    if (decoder.isFinished() && decoder.textureFrameQueue.empty()) {
        return -1;
    }

    ssize_t timeNextUpdate = decoder.updateFrame(mCurrentTextureFrame);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(mVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mVideoFrameTexture);

    videoShader->activate();
    glUniform1i(glGetUniformLocation(videoShader->shaderProgramID, "videoFrame"), 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, outputVideoParams.width, outputVideoParams.height, GL_RGB, GL_UNSIGNED_BYTE, mCurrentTextureFrame.data);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    return timeNextUpdate;
}

const std::string VideoPlayer::videoVertexShader   = "video.vs";
const std::string VideoPlayer::videoFragmentShader = "video.fs";

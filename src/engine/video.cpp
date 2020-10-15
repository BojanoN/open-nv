#include "video.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

// clang-format off
float VideoPlayer::vertices[16] = {
    -1.0,  1.0, 0.0, 0.0,
     1.0,  1.0, 1.0, 0.0,
     1.0, -1.0, 1.0, 1.0,
    -1.0, -1.0, 0.0, 1.0
};

unsigned int VideoPlayer::elements[6] = {
     0, 1, 2,
     2, 3, 0
};
// clang-format on

// TODO: serve the video shader from some type of cache or manager to avoid recompiling the shaders
// every time we try to play a file
VideoPlayer::VideoPlayer()
    : videoShader("./res/shader/video.vs", "./res/shader/video.fs")

{
}

int VideoPlayer::open(const char* path)
{

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(VideoPlayer::vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int err = decoder.open(path);
    if (err < 0) {
        log_error("Unable to open video file for decoding: %s", LibAVVideoDecoder::getError(err));
        glDeleteBuffers(1, &mVBO);
        glDeleteBuffers(1, &mEBO);
        glDeleteVertexArrays(1, &mVAO);
        return -1;
    }

    glGenTextures(1, &mVideoFrameTexture);
    glBindTexture(GL_TEXTURE_2D, mVideoFrameTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, decoder.getWidth(), decoder.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return 0;
};

bool VideoPlayer::update()
{

    if (decoder.isFinished()) {
        return false;
    }

    decoder.updateFrame(mCurrentTextureFrame);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, decoder.getWidth(), decoder.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, mCurrentTextureFrame.data);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

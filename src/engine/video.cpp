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
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLuint videoFrameTexture;

    int err = decoder.open(path);
    if (err < 0) {
        log_error("Unable to open video file for decoding: %s", LibAVVideoDecoder::getError(err));
        return -1;
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(VideoPlayer::vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &videoFrameTexture);
    glBindTexture(GL_TEXTURE_2D, videoFrameTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, decoder.getWidth(), decoder.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return 0;
};

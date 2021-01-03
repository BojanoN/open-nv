#include "gl_renderer.hpp"

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
// clang-format on

namespace Render {

VertexBufferHandle OpenGLRenderer::createVertexBuffer(const void* data, size_t size)
{
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    // TODO: implement a handle system
}

void OpenGLRenderer::clearScreen()
{
    // TODO: take the renderer state into account; e.g. dont clean depth if its not enabled
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::drawTriangleArray(size_t count, size_t offset)
{
    glDrawArrays(GL_TRIANGLES, offset, count);
}

void OpenGLRenderer::drawTrianglesIndexed(size_t count, size_t offset)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, offset);
}

};

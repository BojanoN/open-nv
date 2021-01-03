#include "gl_renderer.hpp"

namespace Render {

ResourceHandle OpenGLRenderer::createVertexBuffer(const void* data, size_t size)
{
    ResourceHandle vbufHandle = mVertexBufferContainer.create();
    if (!vbufHandle.valid()) {
        return vbufHandle;
    }

    GLuint& VBO = mVertexBufferContainer.get(vbufHandle);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return vbufHandle;
}

ResourceHandle OpenGLRenderer::createElementBuffer(const void* data, size_t size)
{
    ResourceHandle ebufHandle = mElementBufferContainer.create();
    if (!ebufHandle.valid()) {
        return ebufHandle;
    }

    GLuint& EBO = mElementBufferContainer.get(ebufHandle);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return ebufHandle;
}

ResourceHandle OpenGLRenderer::createVertexArray()
{
    ResourceHandle varrHandle = mVertexArrayContainer.create();
    if (!varrHandle.valid()) {
        return varrHandle;
    }

    GLuint& VAO = mVertexArrayContainer.get(varrHandle);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    return varrHandle;
}

void OpenGLRenderer::destroyVertexBuffer(ResourceHandle vbufHandle)
{
    if (!mVertexBufferContainer.isValid(vbufHandle)) {
        return;
    }

    GLuint& VBO = mVertexBufferContainer.get(vbufHandle);

    glDeleteBuffers(1, &VBO);
    mVertexBufferContainer.release(vbufHandle);

    return;
}

void OpenGLRenderer::destroyElementBuffer(ResourceHandle ebufHandle)
{
    if (!mElementBufferContainer.isValid(ebufHandle)) {
        return;
    }

    GLuint& EBO = mElementBufferContainer.get(ebufHandle);

    glDeleteBuffers(1, &EBO);
    mElementBufferContainer.release(ebufHandle);

    return;
}

void OpenGLRenderer::destroyVertexArray(ResourceHandle varrHandle)
{
    if (!mVertexArrayContainer.isValid(varrHandle)) {
        return;
    }

    GLuint& VAO = mVertexArrayContainer.get(varrHandle);

    glDeleteVertexArrays(1, &VAO);
    mVertexArrayContainer.release(varrHandle);

    return;
}

void OpenGLRenderer::setVertexBuffer(const ResourceHandle vbufHandle)
{
    if (!mVertexBufferContainer.isValid(vbufHandle)) {
        return;
    }

    GLuint& VBO = mVertexBufferContainer.get(vbufHandle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    return;
}

void OpenGLRenderer::setElementBuffer(const ResourceHandle ebufHandle)
{
    if (!mElementBufferContainer.isValid(ebufHandle)) {
        return;
    }

    GLuint& EBO = mElementBufferContainer.get(ebufHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    return;
}

void OpenGLRenderer::setVertexArray(const ResourceHandle varrHandle)
{
    if (!mElementBufferContainer.isValid(varrHandle)) {
        return;
    }

    GLuint& VAO = mVertexArrayContainer.get(varrHandle);
    glBindVertexArray(VAO);

    return;
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

void OpenGLRenderer::drawTrianglesIndexed(size_t count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

};

#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
// clang-format on

#include <render/renderer.hpp>
#include <util/handle_container.hpp>

namespace Render {

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() {};

    virtual ResourceHandle createVertexBuffer(const void* data, size_t size);
    virtual ResourceHandle createVertexArray();
    virtual ResourceHandle createElementBuffer(const void* data, size_t size);

    virtual void destroyVertexBuffer(ResourceHandle vertexBufferHandle);
    virtual void destroyVertexArray(ResourceHandle vertexArrayHandle);
    virtual void destroyElementBuffer(ResourceHandle elementBufferHandle);

    virtual void setVertexBuffer(const ResourceHandle vertexBufferHandle);
    virtual void setVertexArray(const ResourceHandle vertexArrayHandle);
    virtual void setElementBuffer(const ResourceHandle elementBufferHandle);

    virtual void clearScreen();
    virtual void drawTriangleArray(size_t count, size_t offset);
    virtual void drawTrianglesIndexed(size_t count);

    virtual void createShaderProgramFromPath(const char* vertexShaderPath, const char* fragmentShaderPath);
    virtual void createShaderProgramFromSource(const char* vertexShaderSource, const char* fragmentShaderSource);

    virtual ResourceHandle loadTexture(const Texture2D& texture);
    virtual void           setTexture(const ResourceHandle textureHandle, size_t offset);

    virtual void setRendererState(const RendererState* state);

private:
    typedef HandleContainer<GLuint, HandleResourceType::VertexBuffer>  GLVertexBufferHandleContainer;
    typedef HandleContainer<GLuint, HandleResourceType::ElementBuffer> GLElementBufferHandleContainer;
    typedef HandleContainer<GLuint, HandleResourceType::VertexArray>   GLVertexArrayHandleContainer;

    GLVertexBufferHandleContainer  mVertexBufferContainer;
    GLElementBufferHandleContainer mElementBufferContainer;
    GLVertexArrayHandleContainer   mVertexArrayContainer;
};

};

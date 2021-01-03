#pragma once

#include <render/renderer.hpp>

#define NO_RENDERER_OBJECTS 4096

namespace Render {

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer() {};
    ~OpenGLRenderer() {};

    virtual VertexBuffer*  createVertexBuffer(const void* data, size_t size);
    virtual VertexArray*   createVertexArray(VertexBuffer* buffers, size_t noBuffers);
    virtual ElementBuffer* createElementBuffer(const void* data, size_t size);

    virtual void setVertexBuffer(const VertexBuffer* buffer);
    virtual void setVertexArray(const VertexArray* array);
    virtual void setElementBuffer(const ElementBuffer* buffer);

    virtual void clearScreen();
    virtual void drawTriangleArray(size_t count, size_t offset);
    virtual void drawTrianglesIndexed(size_t count, size_t offset);

    virtual void createShaderProgramFromPath(const char* vertexShaderPath, const char* fragmentShaderPath);
    virtual void createShaderProgramFromSource(const char* vertexShaderSource, const char* fragmentShaderSource);

    virtual void loadTexture(const Texture2D& texture);
    virtual void setTexture(const Texture2D& texture, size_t offset);

    virtual void setRendererState(const RendererState* state);

private:
    GLUint       mVertexBuffers[NO_RENDERER_OBJECTS]             = { 0 };
    unsigned int mVertexBufferHandleCounter[NO_RENDERER_OBJECTS] = { 0 };

    GLUint       mVertexArrays[NO_RENDERER_OBJECTS]             = { 0 };
    unsigned int mVertexArrayHandleCounter[NO_RENDERER_OBJECTS] = { 0 };

    GLUint       mElementBuffers[NO_RENDERER_OBJECTS]             = { 0 };
    unsigned int mElementBufferHandleCounter[NO_RENDERER_OBJECTS] = { 0 };
};

};

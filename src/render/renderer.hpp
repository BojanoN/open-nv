#pragma once

#include <cstddef>
#include <error/error.hpp>
#include <resources/texture2D.hpp>

namespace Render {

typedef unsigned long VertexBufferHandle;
typedef unsigned long ElementBufferHandle;
typedef unsigned long VertexArrayHandle;

class ShaderParameter {
public:
    ~ShaderParameter() {};

protected:
    ShaderParameter();
};

class ShaderProgram {
public:
    ~ShaderProgram() {};

    //Err::Error setInt

protected:
    ShaderProgram();
};

class RendererState {
public:
    ~RendererState() {};

protected:
    RendererState() {};
};

class Renderer {
public:
    Renderer() {};
    virtual ~Renderer() {};

    virtual VertexBufferHandle  createVertexBuffer(const void* data, size_t size)                = 0;
    virtual VertexArrayHandle   createVertexArray(VertexBufferHandle* buffers, size_t noBuffers) = 0;
    virtual ElementBufferHandle createElementBuffer(const void* data, size_t size)               = 0;

    virtual void setVertexBuffer(const VertexBufferHandle buffer)   = 0;
    virtual void setVertexArray(const VertexArrayHandle array)      = 0;
    virtual void setElementBuffer(const ElementBufferHandle buffer) = 0;

    virtual void clearScreen()                                     = 0;
    virtual void drawTriangles(size_t count, size_t offset)        = 0;
    virtual void drawTrianglesIndexed(size_t count, size_t offset) = 0;

    virtual void createShaderProgramFromPath(const char* vertexShaderPath, const char* fragmentShaderPath)       = 0;
    virtual void createShaderProgramFromSource(const char* vertexShaderSource, const char* fragmentShaderSource) = 0;

    virtual void loadTexture(const Texture2D& texture)               = 0;
    virtual void setTexture(const Texture2D& texture, size_t offset) = 0;

    virtual void setRendererState(const RendererState* state) = 0;
};

};

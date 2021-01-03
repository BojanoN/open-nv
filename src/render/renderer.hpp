#pragma once

#include <cstddef>
#include <error/error.hpp>
#include <resources/handle_resource.hpp>
#include <resources/texture2D.hpp>

namespace Render {

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

    virtual ResourceHandle createVertexBuffer(const void* data, size_t size)  = 0;
    virtual ResourceHandle createVertexArray()                                = 0;
    virtual ResourceHandle createElementBuffer(const void* data, size_t size) = 0;

    virtual void destroyVertexBuffer(ResourceHandle vertexBufferHandle)   = 0;
    virtual void destroyVertexArray(ResourceHandle vertexArrayHandle)     = 0;
    virtual void destroyElementBuffer(ResourceHandle elementBufferHandle) = 0;

    virtual void setVertexBuffer(const ResourceHandle vertexBufferHandle)   = 0;
    virtual void setVertexArray(const ResourceHandle vertexArrayHandle)     = 0;
    virtual void setElementBuffer(const ResourceHandle elementBufferHandle) = 0;

    virtual void clearScreen()                              = 0;
    virtual void drawTriangles(size_t count, size_t offset) = 0;
    virtual void drawTrianglesIndexed(size_t count)         = 0;

    virtual void createShaderProgramFromPath(const char* vertexShaderPath, const char* fragmentShaderPath)       = 0;
    virtual void createShaderProgramFromSource(const char* vertexShaderSource, const char* fragmentShaderSource) = 0;

    virtual ResourceHandle loadTexture(const Texture2D& texture)                         = 0;
    virtual void           setTexture(const ResourceHandle textureHandle, size_t offset) = 0;

    virtual void setRendererState(const RendererState* state) = 0;
};

};

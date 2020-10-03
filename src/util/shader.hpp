#pragma once

#include "logc/log.h"

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <math/mat2.hpp>
#include <math/mat3.hpp>
#include <math/mat4.hpp>
#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/vec4.hpp>

#include <cstring>
#include <errno.h>
#include <fstream>
#include <sstream>

class Shader {
public:
    Shader(const char* vShaderPath, const char* fShaderPath)
    {
        std::string vertexShaderSource;
        std::string fragShaderSource;

        unsigned int vShaderID, fShaderID;
        GLint        err;
        size_t       size;

        std::ifstream vShaderFile { vShaderPath };

        if (vShaderFile.fail()) {
            log_fatal("Cannot open vertex shader file: %s", strerror(errno));
            throw 1;
        }

        vShaderFile.seekg(0, std::ios::end);
        size = vShaderFile.tellg();
        vertexShaderSource.resize(size);
        vShaderFile.seekg(0);
        vShaderFile.read(&vertexShaderSource[0], size);

        std::ifstream fShaderFile { fShaderPath };

        if (fShaderFile.fail()) {
            log_fatal("Cannot open fragment shader file: %s", strerror(errno));
            throw 1;
        }

        fShaderFile.seekg(0, std::ios::end);
        size = fShaderFile.tellg();
        fragShaderSource.resize(size);
        fShaderFile.seekg(0);
        fShaderFile.read(&fragShaderSource[0], size);

        // Shader compilation
        const char* glAutismTmpV = vertexShaderSource.c_str();
        const char* glAutismTmpF = fragShaderSource.c_str();

        vShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShaderID, 1, &glAutismTmpV, NULL);
        glCompileShader(vShaderID);

        glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &err);

        if (err == 0) {
            printCompilationError(vShaderID, "vertex");
        }

        fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShaderID, 1, &glAutismTmpF, NULL);
        glCompileShader(fShaderID);
        glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &err);

        if (err == 0) {
            printCompilationError(fShaderID, "fragment");
        }

        // Program linking
        this->shaderProgramID = glCreateProgram();

        glAttachShader(this->shaderProgramID, vShaderID);
        glAttachShader(this->shaderProgramID, fShaderID);

        glLinkProgram(this->shaderProgramID);
        glGetProgramiv(this->shaderProgramID, GL_LINK_STATUS, &err);

        if (err == 0) {
            printLinkingError(this->shaderProgramID);
        }

        glDeleteShader(vShaderID);
        glDeleteShader(fShaderID);
    }

    void activate()
    {
        glUseProgram(this->shaderProgramID);
    }

    void setVec2(std::string& varName, Vector2& vec)
    {
        GLint u = glGetUniformLocation(this->shaderProgramID, varName.c_str());
        glUniform2fv(u, 1, reinterpret_cast<GLfloat*>(&vec));
    }

    void setVec3(std::string& varName, Vector3& vec)
    {
        GLint u = glGetUniformLocation(this->shaderProgramID, varName.c_str());
        glUniform3fv(u, 1, reinterpret_cast<GLfloat*>(&vec));
    }

    void setVec4(std::string& varName, Vector4& vec)
    {
        GLint u = glGetUniformLocation(this->shaderProgramID, varName.c_str());
        glUniform4fv(u, 1, reinterpret_cast<GLfloat*>(&vec));
    }

    void setMat2(std::string& varName, Matrix22& mat, bool transpose)
    {
        GLint u = glGetUniformLocation(this->shaderProgramID, varName.c_str());
        glUniformMatrix2fv(u, 1, transpose, reinterpret_cast<GLfloat*>(&mat));
    }

    void setMat3(std::string& varName, Matrix33& mat, bool transpose)
    {
        GLint u = glGetUniformLocation(this->shaderProgramID, varName.c_str());
        glUniformMatrix3fv(u, 1, transpose, reinterpret_cast<GLfloat*>(&mat));
    }

    void setMat4(std::string& varName, Matrix44& mat, bool transpose)
    {
        GLint u = glGetUniformLocation(this->shaderProgramID, varName.c_str());
        glUniformMatrix4fv(u, 1, transpose, reinterpret_cast<GLfloat*>(&mat));
    }

    void setMat4(const char* varName, Matrix44& mat, bool transpose)
    {
        GLint u = glGetUniformLocation(this->shaderProgramID, varName);
        glUniformMatrix4fv(u, 1, transpose, reinterpret_cast<GLfloat*>(&mat));
    }

    unsigned int shaderProgramID;

private:
    void printCompilationError(GLuint shaderID, const char* type)
    {
        GLchar msg[512];
        glGetShaderInfoLog(shaderID, 512, NULL, msg);

        log_error("Shader compilation failed: %s, type: %s", msg, type);
    }

    void printLinkingError(GLuint ID)
    {
        GLchar msg[512];
        glGetProgramInfoLog(ID, 512, NULL, msg);

        log_error("Shader program linking failed: %s", msg);
    }
};

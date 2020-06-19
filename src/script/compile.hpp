#pragma once

#include "context.hpp"
#include "opcode.hpp"
#include "types/base.hpp"
#include <cstdlib>
#include <string.h>
#include <vector>

#define DEFAULT_ARR_SIZE 512

namespace Script {

class Node;

class CompiledScript {

    // TODO: fix out of bounds read error

public:
    CompiledScript()
        : currentSize(0)
        , currentReadOffset(0)
        , capacity(DEFAULT_ARR_SIZE)
    {
        bytecode = new uint8_t[DEFAULT_ARR_SIZE];
    }

    ~CompiledScript()
    {
        delete[] bytecode;
    }

    bool write(uint8_t* source, uint32_t n)
    {
        if ((currentSize + n) > capacity) {
            resize(n);
        }

        memcpy(bytecode + currentSize, source, n);
        currentSize += n;

        return true;
    }

    bool writeAt(uint32_t offset, uint8_t* source, uint32_t n)
    {

        if ((offset + n) > capacity) {
            return false;
        }

        memcpy(bytecode + offset, source, n);

        return true;
    }

    bool writeZeros(uint32_t n)
    {
        if ((currentSize + n) > capacity) {
            resize(n);
        }

        bzero(this->bytecode + this->currentSize, n);
        currentSize += n;
        return true;
    }

    bool writeByte(uint8_t value)
    {
        if (this->currentSize + 1 < this->capacity) {
            resize(1);
        }

        bytecode[currentSize++] = value;

        return true;
    }

    uint8_t peekByte()
    {
        return this->bytecode[this->currentReadOffset];
    }

    uint16_t peekShort()
    {
        if (currentReadOffset + sizeof(uint16_t) > currentSize) {
            return 0;
        }

        return *((uint16_t*)(bytecode + currentReadOffset));
    }

    uint32_t peekLong()
    {
        if (currentReadOffset + sizeof(uint32_t) > currentSize) {
            return 0;
        }

        return *((uint32_t*)(bytecode + currentReadOffset));
    }

    uint32_t readLong()
    {
        uint32_t ret = peekLong();
        currentReadOffset += sizeof(uint32_t);
        return ret;
    }

    uint16_t readShort()
    {
        uint16_t ret = peekShort();
        currentReadOffset += sizeof(uint16_t);
        return ret;
    }
    uint8_t readByte()
    {
        uint8_t ret = peekByte();
        currentReadOffset++;
        return ret;
    }

    bool jump(uint32_t offset)
    {
        if (offset + currentReadOffset > currentSize) {
            return false;
        }

        currentReadOffset += offset;
        return true;
    }

    bool readString(uint32_t len, char* dest)
    {
        if (len + currentReadOffset > currentSize) {
            return false;
        }

        memcpy(dest, bytecode + currentReadOffset, len);
        currentReadOffset += len;

        return true;
    }

    bool isBeforeOffset(uint32_t offset)
    {
        return currentReadOffset < offset;
    }

    bool hasMoreBytes()
    {
        return currentReadOffset < currentSize;
    }

    uint32_t getReadOffset()
    {
        return currentReadOffset;
    }

    uint32_t getSize()
    {
        return currentSize;
    };

    Value getLocalVariable(uint16_t index)
    {
        Value ret;

        // TODO: local var lookup

        return ret;
    }

    void print();

private:
    uint8_t* bytecode;
    uint32_t currentSize;
    uint32_t capacity;

    uint32_t currentReadOffset;

    void resize(int n)
    {
        capacity += DEFAULT_ARR_SIZE * ((n + capacity) / capacity);
        //        bytecode = (uint8_t*)std::realloc(bytecode, capacity);

        if (bytecode == NULL) {
            throw std::runtime_error("Realloc fail");
        }
    }
};

class Compiler {
public:
    Compiler(std::vector<Node*>* n)
        : nodes(n) {};

    CompiledScript* compile();

private:
    Context             ctx;
    std::vector<Node*>* nodes;

    int compileNode(Node* node, CompiledScript* out);

    int compileBinaryExpr(Node* node, CompiledScript* out);
    int compileAssignment(Node* node, CompiledScript* out);
    int compileGroupingExpr(Node* node, CompiledScript* out);
    int compileLiteralExpr(Node* node, CompiledScript* out);
    int compileFunctionCallExpr(Node* node, CompiledScript* out);
    int compileBlock(Node* node, CompiledScript* out);

    int compileScriptName(Node* node, CompiledScript* out);
    int compileBlocktype(Node* node, CompiledScript* out);
    int compileExpressionStatement(Node* node, CompiledScript* out);
    int compileIfStatement(Node* node, CompiledScript* out);
    int compileVariable(Node* node, CompiledScript* out);

    int error(std::string cause)
    {
        log_fatal("Compile error: %s ", cause.c_str());

        return -1;
    }
};
}

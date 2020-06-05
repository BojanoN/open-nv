#pragma once

#include "context.hpp"
#include "opcode.hpp"
#include <cstdlib>
#include <string.h>
#include <vector>

#define DEFAULT_ARR_SIZE 512

namespace Script {

class Node;

class CompiledScript {

public:
    CompiledScript()
        : currentSize(0)
        , capacity(DEFAULT_ARR_SIZE)
    {
        bytecode = new uint8_t[DEFAULT_ARR_SIZE];
    }

    ~CompiledScript()
    {
        delete bytecode;
    }

    bool write(uint8_t* source, uint32_t n)
    {
        if ((currentSize + n) > capacity) {
            capacity += DEFAULT_ARR_SIZE * (n / capacity);
            bytecode = (uint8_t*)std::realloc(bytecode, capacity);

            if (bytecode == NULL) {
                throw std::runtime_error("Realloc fail");
            }
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
        currentSize += n;

        return true;
    }

    uint32_t getSize() { return currentSize; };

private:
    uint8_t* bytecode;
    uint32_t currentSize;
    uint32_t capacity;
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

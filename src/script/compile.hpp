#pragma once

#include "context.hpp"
#include "opcode.hpp"
#include <vector>

namespace Script {

class Node;

class Compiler {
public:
    Compiler(std::vector<Node*>* n)
        : nodes(n) {};

    void compile();

private:
    Context             ctx;
    std::vector<Node*>* nodes;
    std::vector<Opcode> bytecode;

    int compileNode(Node* node);

    int compileBinaryExpr(Node* node);
    int compileAssignment(Node* node);
    int compileGroupingExpr(Node* node);
    int compileLiteralExpr(Node* node);
    int compileFunctionCallExpr(Node* node);

    int compileScriptName(Node* node);
    int compileBlocktype(Node* node);
    int compileExpressionStatement(Node* node);
    int compileIfStatement(Node* node);
    int compileVariable(Node* node);

    void emit(OpcodeType type, uint16_t arglen);

    int error(std::string cause)
    {
        log_fatal("Compile error: %s ", cause.c_str());

        return -1;
    }
};
}

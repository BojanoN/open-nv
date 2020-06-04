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

    std::vector<uint8_t>* compile();

private:
    Context             ctx;
    std::vector<Node*>* nodes;

    int compileNode(Node* node, std::vector<uint8_t>* out);

    int compileBinaryExpr(Node* node, std::vector<uint8_t>* out);
    int compileAssignment(Node* node, std::vector<uint8_t>* out);
    int compileGroupingExpr(Node* node, std::vector<uint8_t>* out);
    int compileLiteralExpr(Node* node, std::vector<uint8_t>* out);
    int compileFunctionCallExpr(Node* node, std::vector<uint8_t>* out);

    int compileScriptName(Node* node, std::vector<uint8_t>* out);
    int compileBlocktype(Node* node, std::vector<uint8_t>* out);
    int compileExpressionStatement(Node* node, std::vector<uint8_t>* out);
    int compileIfStatement(Node* node, std::vector<uint8_t>* out);
    int compileVariable(Node* node, std::vector<uint8_t>* out);

    int error(std::string cause)
    {
        log_fatal("Compile error: %s ", cause.c_str());

        return -1;
    }
};
}

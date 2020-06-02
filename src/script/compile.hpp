#pragma once

#include "context.hpp"

namespace Script {
class Compiler {
public:
    Compiler() {};

    void compile();

private:
    Context ctx;

    void compileBinaryExpr();
    void compileAssignment();
    void compileGroupingExpr();
    void compileLiteralExpr();
    void compileFunctionCallExpr();

    void compileExpressionStatement();
    void compileIfStatement();
    void compileVariable();

    void error(std::string cause, Token& tok)
    {
        std::string errorMsg = "Syntax error: ";
        errorMsg += cause;
        errorMsg += " at line " + std::to_string(tok.line);
        errorMsg += " at column " + std::to_string(tok.column);
        throw std::runtime_error(errorMsg);
    }
};
}

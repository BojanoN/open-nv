#pragma once
#include "expr.hpp"

namespace Script {

class Statement {
public:
    Statement() {};
    virtual ~Statement() {};
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(Expr* expr)
        : expression(expr) {};
    ~ExpressionStatement()
    {
        delete expression;
    };

private:
    Expr* expression;
};

class Variable : public Statement {
public:
    Variable(TokenType& type, std::string varName)
        : variableType(type)
        , variableName(varName) {};
    ~Variable() {};

    friend class Parser;

private:
    TokenType   variableType;
    std::string variableName;
};

};

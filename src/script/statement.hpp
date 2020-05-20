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

class DeclarationStatement : public Statement {
public:
    DeclarationStatement(Expr* expr, std::string varName)
        : expression(expr)
        , variableName(varName) {};

    ~DeclarationStatement() { delete expression; };

private:
    Expr*       expression;
    std::string variableName;
};

};

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
    ~ExpressionStatement() {};

private:
    Expr* expression;
};

};

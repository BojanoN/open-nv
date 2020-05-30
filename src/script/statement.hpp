#pragma once
#include <string>

#include "tokenizer.hpp"

namespace Script {

class Expr;
class Context;

class Statement {
public:
    Statement() {};
    virtual ~Statement() {};

    virtual void execute(Context& context) = 0;
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(Expr* expr)
        : expression(expr) {};
    ~ExpressionStatement();

    void execute(Context& context);

private:
    Expr* expression;
};

class IfStatement : public Statement {
public:
    IfStatement(Expr* expr, Statement* ifBod, Statement* elseBod)
        : condition(expr)
        , body(ifBod)
        , elseBody(elseBod) {};
    ~IfStatement();

    void execute(Context& context) {};

private:
    Expr*      condition;
    Statement* body;
    Statement* elseBody;
};

class Variable : public Statement {
public:
    Variable(TokenType& type, std::string varName)
        : variableType(type)
        , variableName(varName) {};
    ~Variable() {};

    friend class Parser;
    void execute(Context& context);

private:
    TokenType   variableType;
    std::string variableName;
};

};

#pragma once
#include "tokenizer.hpp"
#include <iostream>

namespace Script {

struct ExprValue {

public:
    ExprValue(TokenType t)
        : i(0)
        , f(0.)
        , s("")
        , type(t)
    {
    }

    ExprValue()
        : i(0)
        , f(0.)
        , s("")
        , type(TokenType::Identifier)
    {
    }

    int32_t     i;
    float       f;
    bool        b;
    std::string s;
    TokenType   type;

    void print()
    {
        std::cout << s;
    }
};

class Expr {
public:
    Expr() {};
    virtual ~Expr() {};
    virtual void      print() = 0;
    virtual ExprValue eval()  = 0;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(Token& opr, Expr* l, Expr* r)
        : op(opr)
        , left(l)
        , right(r)
    {
    }

    ~BinaryExpr()
    {
        delete left;
        delete right;
    }

    void      print();
    ExprValue eval();

private:
    Token op;
    Expr* left;
    Expr* right;
};

class AssignExpr : public Expr {
public:
    void print();

private:
    Token assignee;
    Expr* value;
};

class GroupingExpr : public Expr {
public:
    GroupingExpr(Expr* expr)
        : expression(expr) {};
    ~GroupingExpr()
    {
        delete expression;
    }
    void      print();
    ExprValue eval();

private:
    Expr* expression;
};

class LiteralExpr : public Expr {
public:
    LiteralExpr(std::string val, TokenType t)
        : value(val)
        , type(t)
    {
    }

    void print()
    {
        std::cout << value;
    }
    ExprValue eval();

private:
    std::string value;
    TokenType   type;
};

}

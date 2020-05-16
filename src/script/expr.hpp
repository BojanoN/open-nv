#pragma once
#include "tokenizer.hpp"

namespace Script {

class Expr {
public:
    Expr() {};
    virtual ~Expr() {};
    virtual void print() = 0;
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

    void print();

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
    void print();

private:
    Expr* expression;
};

class LiteralExpr : public Expr {
public:
    LiteralExpr(std::string& val)
        : value(val)
    {
    }

    void print();

private:
    std::string value;
};

}

#pragma once
#include "tokenizer.hpp"

namespace Script {

class Expr {
public:
    Expr() {};
    virtual ~Expr() {};
    //   virtual void parseExpr(Tokenizer& tokenizer);
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

    void parseExpr(Tokenizer& tokenizer);

private:
    Token op;
    Expr* left;
    Expr* right;
};

class AssignExpr : public Expr {
public:
    void parseExpr(Tokenizer& tokenizer);

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
    void parseExpr(Tokenizer& tokenizer);

private:
    Expr* expression;
};

class LiteralExpr : public Expr {
public:
    LiteralExpr(std::string& val)
        : value(val)
    {
    }

    void parseExpr(Tokenizer& tokenizer);

private:
    std::string value;
};

}

#pragma once

#include "misc.hpp"
#include "tokenizer.hpp"
#include <iostream>

namespace Script {

class Context;
class Object;

class Expr {
public:
    Expr() {};
    virtual ~Expr() {};
    virtual void    print()                = 0;
    virtual Object* eval(Context& context) = 0;
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

    void    print();
    Object* eval(Context& context);

private:
    Token op;
    Expr* left;
    Expr* right;
};

class Assignment : public Expr {
public:
    Assignment(std::string var, Expr* expr)
        : variable(var)
        , expression(expr) {};
    ~Assignment()
    {
        delete expression;
    };

    void print() { }

    Object* eval(Context& context);

private:
    std::string variable;
    Expr*       expression;
};

class GroupingExpr : public Expr {
public:
    GroupingExpr(Expr* expr)
        : expression(expr) {};
    ~GroupingExpr()
    {
        delete expression;
    }
    void    print();
    Object* eval(Context& context);

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
    Object* eval(Context& context);

private:
    std::string value;
    TokenType   type;
};

class FunctionCallExpr : public Expr {
public:
    FunctionCallExpr(std::string name, std::vector<Expr*>& args)
        : functionName(name)
        , arguments(args)
    {
    }

    ~FunctionCallExpr()
    {
        for (Expr* e : arguments) {
            delete e;
        }
    };

    void print()
    {
        std::cout << "asdf";
    }
    Object* eval(Context& context);

private:
    std::string        functionName;
    std::vector<Expr*> arguments;
};

}

#pragma once

#include "misc.hpp"
#include "tokenizer.hpp"
#include <iostream>

namespace Script {

class Context;
class Object;

#define NODE_ENUM          \
    X(BinaryExpr)          \
    X(Assignment)          \
    X(GroupingExpr)        \
    X(LiteralExpr)         \
    X(FunctionCallExpr)    \
    X(ExpressionStatement) \
    X(IfStatement)         \
    X(Variable)

#define X(name) name,

enum class NodeType : uint8_t {
    NODE_ENUM
};

#undef X

constexpr const char* NodeEnumToString(NodeType e) noexcept
{
#define X(name)            \
    case (NodeType::name): \
        return #name;

    switch (e) {
        NODE_ENUM
    }
#undef X
}

class Node {
public:
    Node(NodeType t)
        : type(t) {};
    virtual ~Node() {};
    virtual void print() = 0;
    NodeType     type;
};

class BinaryExpr : public Node {
public:
    BinaryExpr(Token& opr, Node* l, Node* r)
        : op(opr)
        , left(l)
        , right(r)
        , Node(NodeType::BinaryExpr)
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
    Node* left;
    Node* right;
};

class Assignment : public Node {
public:
    Assignment(std::string var, Node* expr)
        : variable(var)
        , expression(expr)
        , Node(NodeType::Assignment) {};

    ~Assignment()
    {
        delete expression;
    };

    void print()
    {
        std::cout << "(set " << variable << " to ";
        expression->print();
        std::cout << ")\n";
    }

private:
    std::string variable;
    Node*       expression;
};

class GroupingExpr : public Node {
public:
    GroupingExpr(Node* expr)
        : expression(expr)
        , Node(NodeType::GroupingExpr) {};
    ~GroupingExpr()
    {
        delete expression;
    }
    void print();

private:
    Node* expression;
};

class LiteralExpr : public Node {
public:
    LiteralExpr(std::string val, TokenType t)
        : value(val)
        , type(t)
        , Node(NodeType::LiteralExpr)

    {
    }

    void print()
    {
        std::cout << value;
    }

private:
    std::string value;
    TokenType   type;
};

class FunctionCallExpr : public Node {
public:
    FunctionCallExpr(std::string name, std::vector<Node*>& args)
        : functionName(name)
        , arguments(args)
        , Node(NodeType::FunctionCallExpr)

    {
    }

    ~FunctionCallExpr()
    {
        for (Node* e : arguments) {
            delete e;
        }
    };

    void print()
    {
        std::cout << "(" << functionName << " ";
        for (Node* n : arguments) {
            n->print();
        }
        std::cout << ")\n";
    }

private:
    std::string        functionName;
    std::vector<Node*> arguments;
};

class ExpressionStatement : public Node {
public:
    ExpressionStatement(Node* expr)
        : expression(expr)
        , Node(NodeType::ExpressionStatement) {};

    ~ExpressionStatement()
    {
        delete this->expression;
    };

    void print()
    {
        expression->print();
    }

private:
    Node* expression;
};

class IfStatement : public Node {
public:
    IfStatement(Node* expr, Node* ifBod, std::vector<Node*>& elifBod, Node* elseBod)
        : condition(expr)
        , body(ifBod)
        , elseifBody(elifBod)
        , elseBody(elseBod)
        , Node(NodeType::IfStatement) {};
    ~IfStatement()
    {
        delete condition;
        delete body;
        if (elseBody != nullptr) {
            delete elseBody;
        }

        for (Node* n : elseifBody) {
            delete n;
        }
    };

    void print()
    {
        std::cout << "(if ";
        condition->print();
        body->print();
        if (elseBody != nullptr) {
            elseBody->print();
        } else {
            std::cout << "null";
        }
        std::cout << ")\n";
    }

private:
    Node*              condition;
    Node*              body;
    std::vector<Node*> elseifBody;
    Node*              elseBody;
};

class Variable : public Node {
public:
    Variable(TokenType& type, std::string varName)
        : variableType(type)
        , variableName(varName)
        , Node(NodeType::Variable) {};
    ~Variable() {};

    void print()
    {
        std::cout << "(" << TokenEnumToString(variableType) << " " << variableName << ")\n";
    }

private:
    TokenType   variableType;
    std::string variableName;
};

}

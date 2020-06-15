#pragma once

#include "misc.hpp"
#include "tokenizer.hpp"
#include "types/base.hpp"
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
    X(ScriptName)          \
    X(Blocktype)           \
    X(Block)               \
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

    Node* expression;
};

class LiteralExpr : public Node {
public:
    LiteralExpr(std::string val, Type t)
        : value(val)
        , type(t)
        , Node(NodeType::LiteralExpr)

    {
    }

    void print()
    {
        std::cout << value;
    }

    std::string value;
    Type        type;
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

    Node* expression;
};

class ScriptNameStatement : public Node {
public:
    ScriptNameStatement(std::string& n)
        : Node(NodeType::ScriptName)
        , name(n) {};
    ~ScriptNameStatement() {};

    void print()
    {
        std::cout << "(script " << name << ")\n";
    }

    std::string name;
};
class BlockTypeStatement : public Node {
public:
    BlockTypeStatement(std::string& t, std::string& a)
        : Node(NodeType::Blocktype)
        , type(t)
        , arg(a) {};
    ~BlockTypeStatement() {};

    void print()
    {
        std::cout << "(blocktype " << type << " " << arg << ")\n";
    }

private:
    std::string type;
    std::string arg;
};

class IfStatement : public Node {
public:
    IfStatement(Node* expr, Node* ifBod, std::vector<std::pair<Node*, Node*>>& elifBod, Node* elseBod)
        : condition(expr)
        , body(ifBod)
        , elseIfs(elifBod)
        , elseBody(elseBod)
        , Node(NodeType::IfStatement) {};
    ~IfStatement()
    {
        delete condition;
        delete body;
        if (elseBody != nullptr) {
            delete elseBody;
        }

        for (std::pair<Node*, Node*> n : elseIfs) {
            delete n.first;
            delete n.second;
        }
    };

    void print()
    {
        std::cout << "(if ";
        condition->print();
        body->print();

        if (elseIfs.size()) {
            for (std::pair<Node*, Node*> n : elseIfs) {
                std::cout << "(elseif ";
                n.first->print();
                std::cout << " ";
                n.second->print();
                std::cout << ")\n";
            }
        }

        if (elseBody != nullptr) {
            elseBody->print();
        } else {
            std::cout << "null";
        }
        std::cout << ")\n";
    }

    Node*                                condition;
    Node*                                body;
    std::vector<std::pair<Node*, Node*>> elseIfs;
    Node*                                elseBody;
};

class Variable : public Node {
public:
    Variable(Type& type, std::string varName)
        : variableType(type)
        , variableName(varName)
        , Node(NodeType::Variable) {};
    ~Variable() {};

    void print()
    {
        std::cout << "(" << TypeEnumToString(variableType) << " " << variableName << ")\n";
    }

    Type        variableType;
    std::string variableName;
};

class Block : public Node {
public:
    Block(Node* t, std::vector<Node*>* ns)
        : Node(NodeType::Block)
        , type(t)
        , nodes(ns) {};
    ~Block()
    {
        uint32_t size = nodes->size();

        for (uint32_t i = 0; i < size; i++) {
            delete nodes->at(i);
        }

        delete nodes;
        delete type;
    }

    void print()
    {
        for (Node* n : *nodes) {
            n->print();
        }
    }

    std::vector<Node*>* nodes;
    Node*               type;
};

}

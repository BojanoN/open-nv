#pragma once

#include "tokenizer.hpp"
#include "types/base.hpp"
#include <iostream>

namespace Script {

#define NODE_ENUM          \
    X(BinaryExpr)          \
    X(UnaryExpr)           \
    X(Assignment)          \
    X(GroupingExpr)        \
    X(LiteralExpr)         \
    X(FunctionCall)        \
    X(ExpressionStatement) \
    X(IfStatement)         \
    X(ScriptName)          \
    X(Blocktype)           \
    X(ScriptBlock)         \
    X(ReferenceAccess)     \
    X(StatementBlock)      \
    X(ReturnStatement)     \
    X(VariableAccess)      \
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

    default:
        return "Unknown node type";
    }
#undef X
}

#define BLOCKTYPE_ENUM          \
    X(Function)                 \
    X(GameMode)                 \
    X(MenuMode)                 \
    X(OnActivate)               \
    X(OnActorEquip)             \
    X(OnActorUnequip)           \
    X(OnAdd)                    \
    X(OnClose)                  \
    X(OnCombatEnd)              \
    X(OnDeath)                  \
    X(OnDestructionStageChange) \
    X(OnDrop)                   \
    X(OnEquip)                  \
    X(OnFire)                   \
    X(OnGrab)                   \
    X(OnHit)                    \
    X(OnHitWith)                \
    X(OnLoad)                   \
    X(OnMagicEffectHit)         \
    X(OnMurder)                 \
    X(OnNPCActivate)            \
    X(OnOpen)                   \
    X(OnPackageChange)          \
    X(OnPackageDone)            \
    X(OnPackageStart)           \
    X(OnRelease)                \
    X(OnReset)                  \
    X(OnSell)                   \
    X(OnStartCombat)            \
    X(OnTrigger)                \
    X(OnTriggerEnter)           \
    X(OnTriggerLeave)           \
    X(OnUnequip)                \
    X(SayToDone)                \
    X(ScriptEffectFinish)       \
    X(ScriptEffectStart)        \
    X(ScriptEffectUpdate)

#define X(name) name,

enum class BlockType : uint8_t {
    BLOCKTYPE_ENUM
};

#undef X

constexpr const char* BlockTypeEnumToString(BlockType e) noexcept
{
#define X(name)             \
    case (BlockType::name): \
        return #name;

    switch (e) {
        BLOCKTYPE_ENUM
    }
#undef X
}

enum class NodeContext : uint8_t {
    Expression,
    Statement,
    Assignment
};

class Node {
public:
    Node(NodeType t)
        : type(t) {};
    virtual ~Node() {};
    virtual void print() = 0;

    NodeType type;
};

class BinaryExpr : public Node {
public:
    BinaryExpr(Token& opr, Node* l, Node* r)
        : Node(NodeType::BinaryExpr)
        , op(opr)
        , left(l)
        , right(r)
    {
    }

    ~BinaryExpr()
    {
        delete left;
        delete right;
    }

    void print()
    {
        std::cout << "(";
        this->left->print();
        std::cout << " " << TokenEnumToString(this->op.type) << " ";
        this->right->print();
        std::cout << ")\n";
    }

    Token op;
    Node* left;
    Node* right;
};

class UnaryExpr : public Node {
public:
    UnaryExpr(Token& opr, Node* r)
        : Node(NodeType::UnaryExpr)
        , op(opr)
        , right(r)
    {
    }

    void print()
    {
        std::cout << "()\n";
    }

    ~UnaryExpr()
    {
        delete right;
    }

    Token op;
    Node* right;
};

class Assignment : public Node {
public:
    Assignment(Node* var, Node* expr)
        : Node(NodeType::Assignment)
        , variable(var)
        , expression(expr) {};

    ~Assignment()
    {
        delete expression;
        delete variable;
    };

    void print()
    {
        std::cout << "(set ";
        variable->print();
        expression->print();
        std::cout << ")\n";
    }

    Node* variable;
    Node* expression;
};

class GroupingExpr : public Node {
public:
    GroupingExpr(Node* expr)
        : Node(NodeType::GroupingExpr)
        , expression(expr) {};
    ~GroupingExpr()
    {
        delete expression;
    }

    void print()
    {
        this->expression->print();
    }

    Node* expression;
};

class LiteralExpr : public Node {
public:
    LiteralExpr(Token tok, Type t)
        : Node(NodeType::LiteralExpr)
        , value(tok.literal)
        , valueType(t)

    {
        if (t == Type::Identifier) {
            original = tok.original;
        }
    }

    void print()
    {
        std::cout << value;
    }

    std::string value;
    std::string original = "";
    Type        valueType;
};

class FunctionCall : public Node {
public:
    FunctionCall(std::string& name, std::string& ref, std::vector<Node*>& args, NodeContext ctx)
        : Node(NodeType::FunctionCall)
        , functionName(name)
        , reference(ref)
        , arguments(args)
        , context(ctx)
    {
    }

    ~FunctionCall()
    {
        for (Node* e : arguments) {
            delete e;
        }
    };

    void print()
    {
        std::cout << "(";

        if (this->reference.size())
            std::cout << this->reference << '.';

        std::cout << functionName << " ";
        for (Node* n : arguments) {
            n->print();
            std::cout << " ";
        }
        std::cout << ")\n";
    }

    std::string        functionName;
    std::string        reference;
    std::vector<Node*> arguments;
    NodeContext        context;
};

class ReferenceAccess : public Node {
public:
    ReferenceAccess(std::string& ref, std::string& f, NodeContext ctx)
        : Node(NodeType::ReferenceAccess)
        , reference(ref)
        , field(f)
        , context(ctx)
    {
    }

    ~ReferenceAccess() {};

    void print()
    {
        std::cout << "(" << reference << '.' << field << ")\n";
    }

    std::string reference;
    std::string field;
    NodeContext context;
};

class ExpressionStatement : public Node {
public:
    ExpressionStatement(Node* expr)
        : Node(NodeType::ExpressionStatement)
        , expression(expr) {};

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
    BlockTypeStatement(BlockType t, std::string& a)
        : Node(NodeType::Blocktype)
        , blocktype(t)
        , arg(a) {};
    ~BlockTypeStatement() {};

    void print()
    {
        std::cout << "(blocktype " << arg << ")\n";
    }

    BlockType   blocktype;
    std::string arg;
};

class StatementBlock : public Node {
public:
    StatementBlock(std::vector<Node*>* ns)
        : Node(NodeType::StatementBlock)
        , nodes(ns) {};

    ~StatementBlock()
    {
        uint32_t size = nodes->size();

        for (uint32_t i = 0; i < size; i++) {
            delete nodes->at(i);
        }

        delete nodes;
    }

    void print()
    {
        for (Node* n : *nodes) {
            n->print();
        }
    }

    std::vector<Node*>* nodes;
};

class IfStatement : public Node {
public:
    IfStatement(Node* expr, StatementBlock* ifBod, std::vector<std::pair<Node*, StatementBlock*>>& elifBod, StatementBlock* elseBod)
        : Node(NodeType::IfStatement)
        , condition(expr)
        , body(ifBod)
        , elseIfs(elifBod)
        , elseBody(elseBod) {};
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

    Node*                                          condition;
    StatementBlock*                                body;
    std::vector<std::pair<Node*, StatementBlock*>> elseIfs;
    StatementBlock*                                elseBody;
};

class Variable : public Node {
public:
    Variable(Type& type, std::string& varName)
        : Node(NodeType::Variable)
        , variableType(type)
        , variableName(varName) {};
    ~Variable() {};

    void print()
    {
        std::cout << "(" << TypeEnumToString(variableType) << " " << variableName << ")\n";
    }

    Type        variableType;
    std::string variableName;
};

class VariableAccess : public Node {
public:
    VariableAccess(std::string& varName, NodeContext ctx)
        : Node(NodeType::VariableAccess)
        , variableName(varName)
        , context(ctx) {};
    ~VariableAccess() {};

    void print()
    {
        std::cout << variableName;
    }

    std::string variableName;
    NodeContext context;
};

class ScriptBlock : public Node {
public:
    ScriptBlock(Node* t, std::vector<Node*>* ns)
        : Node(NodeType::ScriptBlock)
        , nodes(ns)
        , blocktype(t) {};
    ~ScriptBlock()
    {
        uint32_t size = nodes->size();

        for (uint32_t i = 0; i < size; i++) {
            delete nodes->at(i);
        }

        delete nodes;
        delete blocktype;
    }

    void print()
    {
        for (Node* n : *nodes) {
            n->print();
        }
    }

    std::vector<Node*>* nodes;
    Node*               blocktype;
};

class ReturnStatement : public Node {
public:
    ReturnStatement()
        : Node(NodeType::ReturnStatement) {};
    ~ReturnStatement() { }

    void print()
    {
        std::cout << "return\n";
    }
};

}

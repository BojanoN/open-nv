#include "compile.hpp"
#include "node.hpp"

namespace Script {
int Compiler::compileNode(Node* node)
{
    switch (node->type) {
    case NodeType::BinaryExpr:
        return compileBinaryExpr(node);

    case NodeType::Assignment:
        return compileAssignment(node);

    case NodeType::GroupingExpr:
        return compileGroupingExpr(node);

    case NodeType::LiteralExpr:
        return compileLiteralExpr(node);

    case NodeType::FunctionCallExpr:
        return compileFunctionCallExpr(node);

    case NodeType::IfStatement:
        return compileIfStatement(node);

    case NodeType::ScriptName:
        return compileScriptName(node);

    case NodeType::Blocktype:
        return compileBlocktype(node);

    case NodeType::Variable:
        return compileVariable(node);

    default:
        return error("Unknown node type");
    }
    return 0;
}

void Compiler::emit(OpcodeType type, uint16_t arglen)
{
    Opcode tmp = { type, arglen };
    bytecode.push_back(tmp);
};

int Compiler::compileBinaryExpr(Node* node)
{

    return 0;
};

int Compiler::compileAssignment(Node* node)
{

    return 0;
};

int Compiler::compileGroupingExpr(Node* node)
{
    return compileNode(dynamic_cast<GroupingExpr*>(node)->expression);
};

int Compiler::compileLiteralExpr(Node* node)
{
    return 0;
};

int Compiler::compileFunctionCallExpr(Node* node)
{
    return 0;
};

int Compiler::compileScriptName(Node* node)
{
    ScriptNameStatement* scn = dynamic_cast<ScriptNameStatement*>(node);

    this->ctx.scriptName = scn->name;

    this->emit(OpcodeType::SCRIPTNAME, 0);

    return 0;
};
int Compiler::compileBlocktype(Node* node)
{
    return 0;
};
int Compiler::compileExpressionStatement(Node* node)
{
    return 0;
};
int Compiler::compileIfStatement(Node* node)
{
    return 0;
};
int Compiler::compileVariable(Node* node)
{
    return 0;
};

};

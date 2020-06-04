#include "compile.hpp"
#include "node.hpp"

namespace Script {

std::vector<uint8_t>* Compiler::compile()
{

    std::vector<uint8_t>* ret = new std::vector<uint8_t>();

    uint32_t n = nodes->size();
    uint32_t err;

    for (uint32_t i = 0; i < n; i++) {
        err = compileNode(nodes->at(i));
        if (err) {
            delete ret;
            return nullptr;
        }
    }

    return ret;
}

int Compiler::compileNode(Node* node, std::vector<uint8_t>* out)
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

int Compiler::compileBinaryExpr(Node* node, std::vector<uint8_t>* out)
{

    return 0;
};

int Compiler::compileAssignment(Node* node, std::vector<uint8_t>* out)
{

    return 0;
};

int Compiler::compileGroupingExpr(Node* node, std::vector<uint8_t>* out)
{
    return compileNode(dynamic_cast<GroupingExpr*>(node)->expression);
};

int Compiler::compileLiteralExpr(Node* node, std::vector<uint8_t>* out)
{
    return 0;
};

int Compiler::compileFunctionCallExpr(Node* node, std::vector<uint8_t>* out)
{
    return 0;
};

int Compiler::compileScriptName(Node* node, std::vector<uint8_t>* out)
{
    ScriptNameStatement* scn = dynamic_cast<ScriptNameStatement*>(node);

    this->ctx.scriptName = scn->name;

    return 0;
};
int Compiler::compileBlocktype(Node* node, std::vector<uint8_t>* out)
{
    return 0;
};
int Compiler::compileExpressionStatement(Node* node, std::vector<uint8_t>* out)
{
    return 0;
};
int Compiler::compileIfStatement(Node* node, std::vector<uint8_t>* out)
{
    return 0;
};
int Compiler::compileVariable(Node* node, std::vector<uint8_t>* out)
{
    return 0;
};

};

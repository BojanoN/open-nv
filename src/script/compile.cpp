#include "compile.hpp"
#include "node.hpp"

namespace Script {

CompiledScript* Compiler::compile()
{

    CompiledScript* ret = new CompiledScript();

    uint32_t n = nodes->size();
    uint32_t err;

    for (uint32_t i = 0; i < n; i++) {
        err = compileNode(nodes->at(i), ret);
        if (err) {
            delete ret;
            return nullptr;
        }
    }

    return ret;
}

int Compiler::compileNode(Node* node, CompiledScript* out)
{
    switch (node->type) {
    case NodeType::BinaryExpr:
        return compileBinaryExpr(node, out);

    case NodeType::Assignment:
        return compileAssignment(node, out);

    case NodeType::GroupingExpr:
        return compileGroupingExpr(node, out);

    case NodeType::LiteralExpr:
        return compileLiteralExpr(node, out);

    case NodeType::FunctionCallExpr:
        return compileFunctionCallExpr(node, out);

    case NodeType::IfStatement:
        return compileIfStatement(node, out);

    case NodeType::ScriptName:
        return compileScriptName(node, out);

    case NodeType::Blocktype:
        return compileBlocktype(node, out);

    case NodeType::Variable:
        return compileVariable(node, out);
    case NodeType::Block:
        return compileBlock(node, out);
    default:
        return error("Unknown node type");
    }
    return 0;
}

int Compiler::compileBlock(Node* node, CompiledScript* out)
{
    Block* block = dynamic_cast<Block*>(node);
    int    size  = block->nodes->size();

    uint8_t beg[] = {
        0x10, 0x00, 0x06, 0x00,
        0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    uint8_t end[] = {
        0x11, 0x00, 0x00, 0x00
    };

    uint32_t totalWrite      = out->getSize();
    uint32_t totalBlocksize  = 0;
    uint32_t blocksizeOffset = out->getSize() + 6;

    out->write(beg, sizeof(beg));

    for (int i = 0; i < size; i++) {
        int compiledBytes = compileNode(block->nodes->at(i), out);

        if (compiledBytes < 0) {
            return -1;
        }

        totalBlocksize += (uint32_t)compiledBytes;
    }

    if (!out->writeAt(blocksizeOffset, (uint8_t*)&totalBlocksize, 4)) {
        log_fatal("WriteAt error");
        return -1;
    }
    out->write(end, sizeof(end));

    totalWrite = out->getSize() - totalWrite;

    return totalWrite;
};

int Compiler::compileBinaryExpr(Node* node, CompiledScript* out)
{

    return 0;
};

int Compiler::compileAssignment(Node* node, CompiledScript* out)
{

    return 0;
};

int Compiler::compileGroupingExpr(Node* node, CompiledScript* out)
{
    return compileNode(dynamic_cast<GroupingExpr*>(node)->expression, out);
};

int Compiler::compileLiteralExpr(Node* node, CompiledScript* out)
{
    return 0;
};

int Compiler::compileFunctionCallExpr(Node* node, CompiledScript* out)
{
    return 0;
};

int Compiler::compileScriptName(Node* node, CompiledScript* out)
{
    ScriptNameStatement* scn = dynamic_cast<ScriptNameStatement*>(node);

    this->ctx.scriptName = scn->name;

    uint8_t scn[] = { 0x1D, 0x00, 0x00, 0x00 };

    out->write(scn, sizeof(scn));

    return sizeof(scn);
};
int Compiler::compileBlocktype(Node* node, CompiledScript* out)
{
    return 0;
};
int Compiler::compileExpressionStatement(Node* node, CompiledScript* out)
{
    return 0;
};
int Compiler::compileIfStatement(Node* node, CompiledScript* out)
{
    return 0;
};
int Compiler::compileVariable(Node* node, CompiledScript* out)
{
    return 0;
};

};

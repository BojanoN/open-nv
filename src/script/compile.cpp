#include "compile.hpp"
#include "function.hpp"
#include "node.hpp"
#include "stdio.h"

#define BYTE_SWAP_SHORT(value) \
    (((value)&0xFF) << 8 | ((value)&0xFF00) >> 8)

namespace Script {

void CompiledScript::print()
{
    for (uint32_t i = 0; i < currentSize; i++) {
        printf("%.2x ", bytecode[i]);
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }
    printf("\n");
};

CompiledScript* Compiler::compile()
{

    CompiledScript* ret = new CompiledScript();

    uint32_t n = nodes->size();
    uint32_t err;

    for (uint32_t i = 0; i < n; i++) {
        err = compileNode(nodes->at(i), ret);
        if (err < 0) {
            delete ret;
            return nullptr;
        }
    }

    return ret;
}

int Compiler::compileNode(Node* node, CompiledScript* out)
{

    log_debug("%s", NodeEnumToString(node->type));

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

    case NodeType::ExpressionStatement:
        return compileExpressionStatement(node, out);
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
    LiteralExpr* expr    = dynamic_cast<LiteralExpr*>(node);
    uint32_t     begSize = out->getSize();

    out->writeByte(0x20);
    if (expr->type == Type::Identifier) {
        uint8_t  typeCode;
        uint16_t varIndex;

        // Check if defined locally
        if (ctx.varExists(expr->value)) {

            std::pair<Type, int> varInfo = ctx.getVar(expr->value);
            varIndex                     = varInfo.second;
            switch (varInfo.first) {
            case (Type::Reference):
            case (Type::Float):
                typeCode = 0x66;
                break;
            case (Type::Short):
            case (Type::Integer):
                typeCode = 0x73;
                break;
            default:
                return -1;
                break;
            }
        } else {
            // If not present locally leave for reference checking later
            // TODO: SCRO lookup

            typeCode      = 0x5A;
            uint32_t form = std::stoi(expr->value);
            // Dummy SCRO lookup for now
            varIndex = ctx.SCROLookup(form);
        }
        out->writeByte(typeCode);
        out->write((uint8_t*)&varIndex, sizeof(uint16_t));
    } else {
        out->write((uint8_t*)expr->value.data(), expr->value.size());
    }

    return out->getSize() - begSize;
};

int Compiler::compileFunctionCallExpr(Node* node, CompiledScript* out)
{
    FunctionCallExpr* func    = dynamic_cast<FunctionCallExpr*>(node);
    uint32_t          begSize = out->getSize();

    FunctionInfo& info = FunctionResolver::functions[func->functionName];

    uint16_t funcCode = info.opcode;
    out->write((uint8_t*)&funcCode, sizeof(uint16_t));

    uint32_t paramBytesOffset = out->getSize();
    out->writeZeros(sizeof(uint16_t));

    uint16_t paramCount = func->arguments.size();
    out->write((uint8_t*)&paramCount, sizeof(uint16_t));

    uint16_t paramBytes = 0;

    for (uint32_t i = 0; i < paramCount; i++) {
        int written = compileNode(func->arguments[i], out);
        if (written < 0) {
            return -1;
        }

        paramBytes += (uint32_t)written;
    }

    out->writeAt(paramBytesOffset, (uint8_t*)&paramBytes, sizeof(uint16_t));

    return out->getSize() - begSize;
};

int Compiler::compileScriptName(Node* node, CompiledScript* out)
{
    ScriptNameStatement* scriptName = dynamic_cast<ScriptNameStatement*>(node);

    this->ctx.scriptName = scriptName->name;

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
    ExpressionStatement* expr = dynamic_cast<ExpressionStatement*>(node);
    return compileNode(expr->expression, out);
};
int Compiler::compileIfStatement(Node* node, CompiledScript* out)
{
    IfStatement* ifStmt    = dynamic_cast<IfStatement*>(node);
    uint32_t     begOffset = out->getSize();

    compileNode(ifStmt->condition, out);
    compileNode(ifStmt->body, out);

    uint32_t elifsSize = ifStmt->elseIfs.size();
    if (elifsSize) {
        for (uint32_t i = 0; i < elifsSize; i++) {
            compileNode(ifStmt->elseIfs[i].first, out);
            compileNode(ifStmt->elseIfs[i].second, out);
        }
    }

    if (ifStmt->elseBody != nullptr) {
        compileNode(ifStmt->elseBody, out);
    }

    return out->getSize() - begOffset;
};
int Compiler::compileVariable(Node* node, CompiledScript* out)
{
    Variable* var = dynamic_cast<Variable*>(node);

    this->ctx.declareVar(var->variableName, var->variableType);

    return 0;
};
};

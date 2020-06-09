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
        static_cast<uint8_t>(OutputCodes::BEGIN), 0x00, 0x06, 0x00,
        0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    uint8_t end[] = {
        static_cast<uint8_t>(OutputCodes::END), 0x00, 0x00, 0x00
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
    if (!out->writeAt(blocksizeOffset, (uint8_t*)&totalBlocksize, sizeof(uint32_t))) {
        log_fatal("WriteAt error");
        return -1;
    }

    out->write(end, sizeof(end));

    totalWrite = out->getSize() - totalWrite;

    return totalWrite;
};

int Compiler::compileBinaryExpr(Node* node, CompiledScript* out)
{
    BinaryExpr* expr    = dynamic_cast<BinaryExpr*>(node);
    uint32_t    begSize = out->getSize();

    compileNode(expr->left, out);
    compileNode(expr->right, out);

    // TODO: check if this really is the case
    out->writeByte(static_cast<uint8_t>(OutputCodes::PUSH));
    out->write((uint8_t*)expr->op.literal.data(), expr->op.literal.size());

    return out->getSize() - begSize;
};

int Compiler::compileAssignment(Node* node, CompiledScript* out)
{
    Assignment* expr    = dynamic_cast<Assignment*>(node);
    uint32_t    begSize = out->getSize();

    uint8_t set[]              = { static_cast<uint8_t>(OutputCodes::ASSIGN), 0x00 };
    uint8_t exprLenPlaceholder = { 0x00, 0x00 };

    out->write(set, 2);
    uint16_t nameLen = expr->variable.size();
    out->write(nameLen, sizeof(uint16_t));
    out->write((uint8_t*)expr->variable.data(), nameLen);

    uint32_t exprLenOffset = out->getSize();
    out->write(exprLenPlaceholder, 2);

    int exprLen = compileNode(expr->expression, out);
    if (exprLen < 0) {
        return -1;
    }
    uint16_t exprLenOut = (uint16_t)exprLen;
    out->writeAt(exprLenOffset, (uint8_t*)&exprLenOut, sizeof(exprLenOut));

    return out->getSize() - begSize;
};

int Compiler::compileGroupingExpr(Node* node, CompiledScript* out)
{
    return compileNode(dynamic_cast<GroupingExpr*>(node)->expression, out);
};

int Compiler::compileLiteralExpr(Node* node, CompiledScript* out)
{
    LiteralExpr* expr    = dynamic_cast<LiteralExpr*>(node);
    uint32_t     begSize = out->getSize();

    out->writeByte(static_cast<uint8_t>(OutputCodes::PUSH));
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
                typeCode = static_cast<uint8_t>(OutputCodes::FLOAT_REF_LOCAL);
                break;
            case (Type::Short):
            case (Type::Integer):
                typeCode = static_cast<uint8_t>(OutputCodes::INT_LOCAL);
                break;
            default:
                return -1;
                break;
            }
        } else {
            // If not present locally leave for reference checking later
            // TODO: SCRO lookup

            typeCode          = static_cast<uint8_t>(OutputCodes::REF_GLOBAL);
            std::string& form = expr->value;
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
int Compiler::compileExpressionStatement(Node* node, CompiledScript* out)
{
    ExpressionStatement* expr = dynamic_cast<ExpressionStatement*>(node);
    return compileNode(expr->expression, out);
};
int Compiler::compileIfStatement(Node* node, CompiledScript* out)
{
    IfStatement* ifStmt    = dynamic_cast<IfStatement*>(node);
    uint32_t     begOffset = out->getSize();

    uint8_t ifBegin[] = { static_cast<uint8_t>(OutputCodes::IF), 0x00 };

    int      exprLen, compLen;
    uint16_t exprLenOut, compLenOut, jumpOps;
    uint32_t exprLenOffset, jumpOpsOffset;

    // TODO: check .esm files for source and see what this is supposed to be
    compLenOut = 0;

    exprLen = compileNode(ifStmt->condition, out);
    if (compLen < 0) {
        return -1;
    }
    exprLenOut = (uint16_t)exprLen;
    jumpOps    = exprLenOut + sizeof(uint16_t);
    compLenOut = jumpOps + sizeof(uint16_t);

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

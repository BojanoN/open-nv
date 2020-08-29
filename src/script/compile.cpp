#include "compile.hpp"
#include "function.hpp"
#include "node.hpp"
#include "stdio.h"

#define CHECK_NULL(var)                                            \
    if (var == nullptr) {                                          \
        log_fatal("Null pointer encountered in value " #var "\n"); \
        return -1;                                                 \
    }

namespace Script {

void CompiledScript::print()
{
    for (uint32_t i = 0; i < currentSize; i++) {
        printf("%.2x ", bytecode[i]);
        if ((i + 1) % 4 == 0) {
            printf("\n%.5u: ", i + 1);
        }
    }
    printf("\n");
};

CompiledScript* Compiler::compile(std::vector<Node*>* ns)
{

    this->nodes         = ns;
    CompiledScript* ret = new CompiledScript();

    uint32_t n = nodes->size();
    int      err;

    for (uint32_t i = 0; i < n; i++) {
        err = compileNode(nodes->at(i), ret);
        if (err < 0) {
            delete ret;
            this->nodes = nullptr;
            return nullptr;
        }
    }

    this->nodes = nullptr;

    return ret;
}

int Compiler::compileNode(Node* node, CompiledScript* out)
{

    log_debug("%s", NodeEnumToString(node->type));

    CHECK_NULL(node);

    switch (node->type) {
    case NodeType::BinaryExpr:
        return compileBinaryExpr(node, out);
    case NodeType::Assignment:
        return compileAssignment(node, out);
    case NodeType::GroupingExpr:
        return compileGroupingExpr(node, out);
    case NodeType::LiteralExpr:
        return compileLiteralExpr(node, out);
    case NodeType::FunctionCall:
        return compileFunctionCall(node, out);
    case NodeType::ExpressionStatement:
        return compileExpressionStatement(node, out);
    case NodeType::IfStatement:
        return compileIfStatement(node, out);
    case NodeType::ScriptName:
        return compileScriptName(node, out);
    case NodeType::Variable:
        return compileVariable(node, out);
    case NodeType::StatementBlock:
        return compileStatementBlock(node, out);
    case NodeType::ScriptBlock:
        return compileScriptBlock(node, out);
    case NodeType::ReturnStatement:
        return compileReturnStatement(node, out);
    case NodeType::ReferenceAccess:
        return compileReferenceAccess(node, out);
    case NodeType::VariableAccess:
        return compileVariableAccess(node, out);
    default:
        return error("Unknown node type");
    }
    return 0;
}

int Compiler::compileOnTriggerEnter(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset)
{
    uint8_t  onTriggerEnterID[] = { 0x1A, 0x00 };
    uint16_t argumentCount      = 1;
    uint16_t varIndex           = 0;

    if (blocktype->arg.size() == 0) {
        log_fatal("OnTriggerEnter blocktype requires an argument");
        return -1;
    }

    out->writeAt(blocktypeCodeOffset, onTriggerEnterID, sizeof(uint16_t));
    out->writeShort(argumentCount);

    out->writeByte(ExprCodes::REF_FUNC_PARAM);

    varIndex = ctx.SCROLookup(blocktype->arg);
    if (varIndex == 0) {
        return -1;
    }

    out->writeShort(varIndex);

    return 0;
}

int Compiler::compileOnTrigger(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeIDOffset)
{
    uint8_t  onTriggerEnterID[] = { 0x18, 0x00 };
    uint16_t argumentCount      = 1;
    uint16_t varIndex           = 0;

    if (blocktype->arg.size() == 0) {
        log_fatal("OnTriggerEnter blocktype requires an argument");
        return -1;
    }

    out->writeAt(blocktypeIDOffset, onTriggerEnterID, sizeof(uint16_t));
    out->writeShort(argumentCount);

    out->writeByte(ExprCodes::REF_FUNC_PARAM);

    varIndex = ctx.SCROLookup(blocktype->arg);
    if (varIndex == 0) {
        return -1;
    }

    out->writeShort(varIndex);

    return 0;
}

int Compiler::compileGameMode(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset)
{
    uint8_t gameModeID[] = { 0x00, 0x00 };
    // TODO: check bytecode for argcount
    //uint16_t argumentCount = 0;

    out->writeAt(blocktypeCodeOffset, gameModeID, sizeof(uint16_t));

    return 0;
}

int Compiler::compileBlocktype(Node* node, CompiledScript* out)
{
    BlockTypeStatement* blocktype = dynamic_cast<BlockTypeStatement*>(node);

    uint32_t begSize = out->getSize();
    // blocktype ID placeholder
    out->writeZeros(sizeof(uint16_t));
    // block size placeholder
    out->writeZeros(sizeof(uint32_t));
    int ret;

    switch (blocktype->blocktype) {
    case BlockType::OnTriggerEnter: {
        ret = compileOnTriggerEnter(blocktype, out, begSize);
        break;
    }
    case BlockType::OnTrigger: {
        ret = compileOnTrigger(blocktype, out, begSize);
        break;
    }
    case BlockType::GameMode: {
        ret = compileGameMode(blocktype, out, begSize);
        break;
    }

    default:
        log_fatal("Unknown blocktype %s", BlockTypeEnumToString(blocktype->blocktype));
        return -1;
    }

    if (ret < 0) {
        return -1;
    }

    return out->getSize() - begSize;
}

int Compiler::compileScriptBlock(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    ScriptBlock* block = dynamic_cast<ScriptBlock*>(node);
    int          size  = block->nodes->size();

    uint16_t beg = OutputCodes::BEGIN;

    Opcode end = { OutputCodes::END, 0x00 };

    uint32_t totalWrite = out->getSize();
    out->writeShort(beg);

    uint32_t currentNodeOffset = 0;

    // Variable declarations
    while (block->nodes->at(currentNodeOffset)->type == NodeType::Variable) {
        int compiledBytes = compileNode(block->nodes->at(currentNodeOffset), out);

        if (compiledBytes < 0) {
            return -1;
        }
        currentNodeOffset++;
    }

    int      blocktypeSize;
    uint32_t blocktypeSizeOffset = out->getSize();
    uint32_t totalBlocksize      = 0;

    out->writeZeros(sizeof(uint16_t));

    blocktypeSize = compileBlocktype(block->blocktype, out);
    if (blocktypeSize < 0) {
        return -1;
    }

    out->writeAt(blocktypeSizeOffset, (uint8_t*)&blocktypeSize, sizeof(uint16_t));

    uint32_t blocksizeOffset = blocktypeSizeOffset + sizeof(uint16_t) + sizeof(uint16_t);

    for (int i = currentNodeOffset; i < size; i++) {
        int compiledBytes = compileNode(block->nodes->at(i), out);

        if (compiledBytes < 0) {
            return -1;
        }

        totalBlocksize += (uint32_t)compiledBytes;
    }
    // Take the end opcode into account
    totalBlocksize += sizeof(end);
    if (!out->writeAt(blocksizeOffset, (uint8_t*)&totalBlocksize, sizeof(uint32_t))) {
        log_fatal("WriteAt error");
        return -1;
    }

    out->writeOpcode(end);

    totalWrite = out->getSize() - totalWrite;

    return totalWrite;
};

int Compiler::compileBinaryExpr(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    BinaryExpr* expr    = dynamic_cast<BinaryExpr*>(node);
    uint32_t    begSize = out->getSize();
    int         ret;

    ret = compileNode(expr->left, out);
    if (ret < 0) {
        return -1;
    }

    ret = compileNode(expr->right, out);
    if (ret < 0) {
        return -1;
    }

    out->writeByte(static_cast<uint8_t>(ExprCodes::PUSH));
    out->write((uint8_t*)expr->op.literal.data(), expr->op.literal.size());

    return out->getSize() - begSize;
};

int Compiler::compileAssignment(Node* node, CompiledScript* out)
{

    CHECK_NULL(node);

    Assignment* expr    = dynamic_cast<Assignment*>(node);
    uint32_t    begSize = out->getSize();

    Opcode set = { OutputCodes::ASSIGN, 0x00 };

    out->writeOpcode(set);
    uint32_t lengthOffset = out->getSize() - 2;

    int varLen = compileNode(expr->variable, out);
    if (varLen < 0) {
        return -1;
    }

    uint32_t exprLenOffset = out->getSize();
    // Fill the initial expression len with zeroes
    out->writeShort(0x00);

    int exprLen = compileNode(expr->expression, out);
    if (exprLen < 0) {
        return -1;
    }

    uint16_t exprLenOut = (uint16_t)exprLen;
    out->writeAt(exprLenOffset, (uint8_t*)&exprLenOut, sizeof(exprLenOut));

    set.length = varLen + sizeof(uint16_t) + exprLenOut;
    out->writeAt(lengthOffset, (uint8_t*)&set.length, sizeof(set.length));

    return out->getSize() - begSize;
};

int Compiler::compileGroupingExpr(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    return compileNode(dynamic_cast<GroupingExpr*>(node)->expression, out);
};

int Compiler::compileLiteralExpr(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    LiteralExpr* expr    = dynamic_cast<LiteralExpr*>(node);
    uint32_t     begSize = out->getSize();

    out->writeByte(static_cast<uint8_t>(ExprCodes::PUSH));
    if (expr->valueType == Type::Identifier) {
        uint8_t  typeCode;
        uint16_t varIndex;

        // Check if defined locally
        if (ctx.varExists(expr->value)) {

            std::pair<VariableInfo, bool> varInfo = ctx.getVar(expr->value);
            varIndex                              = varInfo.first.index;
            switch (varInfo.first.type) {
            case (Type::Reference):
            case (Type::Float):
                typeCode = static_cast<uint8_t>(ExprCodes::FLOAT_REF_LOCAL);
                break;
            case (Type::Short):
            case (Type::Integer):
                typeCode = static_cast<uint8_t>(ExprCodes::INT_LOCAL);
                break;
            default:
                return -1;
                break;
            }
        } else {
            // If not present locally leave for reference checking later
            typeCode              = static_cast<uint8_t>(ExprCodes::REF_GLOBAL);
            std::string& editorId = expr->original;
            varIndex              = ctx.SCROLookup(editorId);
            if (varIndex == 0) {
                return -1;
            }
        }
        out->writeByte(typeCode);
        out->writeShort(varIndex);
    } else {
        out->write((uint8_t*)expr->value.data(), expr->value.size());
    }

    return out->getSize() - begSize;
};

int Compiler::compileFunctionCall(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    FunctionCall* func    = dynamic_cast<FunctionCall*>(node);
    uint32_t      begSize = out->getSize();

    FunctionInfo& info = FunctionResolver::getFunctionInfo(func->functionName);

    // Reference function call, inside an expression
    if (func->reference.size()) {
        if (func->context == NodeContext::Expression || func->context == NodeContext::Assignment) {
            out->writeByte(ExprCodes::PUSH);
            out->writeByte(ExprCodes::REF_FUNC_PARAM);
        } else {
            out->writeShort(OutputCodes::REF_ACCESS);
        }
        uint16_t index = ctx.SCROLookup(func->reference);
        out->writeShort(index);
    }
    if (func->context == NodeContext::Expression) {
        out->writeByte(ExprCodes::PUSH);
        out->writeByte(ExprCodes::FUNC_CALL);
    }

    uint16_t funcCode = info.opcode;
    out->writeShort(funcCode);

    uint32_t paramBytesOffset = out->getSize();
    out->writeShort(0x00);

    if (info.paramCount > 0) {

        uint16_t paramCount = func->arguments.size();
        out->writeShort(paramCount);

        uint16_t     paramBytes = sizeof(uint16_t);
        LiteralExpr* literal;

        for (uint32_t i = 0; i < paramCount; i++) {
            // TODO: add information about the number of parameters required for each available function
            // so we can enable nested function calls
            if (func->arguments[i]->type != NodeType::LiteralExpr) {
                log_fatal("Function arguments can only be constant values");
                return -1;
            }

            literal = dynamic_cast<LiteralExpr*>(func->arguments[i]);

            switch (literal->valueType) {
            case (Type::Integer): {
                out->writeByte(ExprCodes::LONG_FUNC_PARAM);
                uint32_t value = std::stol(literal->value);
                out->writeLong(value);
                paramBytes += 1 + sizeof(uint32_t);
                break;
            }
            case (Type::Reference):
            case (Type::Identifier): {
                out->writeByte(ExprCodes::REF_FUNC_PARAM);
                uint16_t index = ctx.SCROLookup(literal->value);
                out->writeShort(index);
                paramBytes += 1 + sizeof(uint16_t);
                break;
            }
            case (Type::Float): {
                out->writeByte(ExprCodes::FLOAT_FUNC_PARAM);
                double value = std::stod(literal->value);
                out->write((uint8_t*)&value, sizeof(double));
                paramBytes += 1 + sizeof(double);
                break;
            }
            default:
                log_fatal("Unknown function parameter %s", TypeEnumToString(literal->valueType));
                return -1;
            }
        }

        out->writeAt(paramBytesOffset, (uint8_t*)&paramBytes, sizeof(uint16_t));
    }

    return out->getSize() - begSize;
};

int Compiler::compileScriptName(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    ScriptNameStatement* scriptName = dynamic_cast<ScriptNameStatement*>(node);

    this->ctx.scriptName = scriptName->name;

    Opcode scn = { OutputCodes::SCRIPTNAME, 0x00 };

    out->writeOpcode(scn);

    return sizeof(scn);
};

int Compiler::compileExpressionStatement(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    ExpressionStatement* expr = dynamic_cast<ExpressionStatement*>(node);
    return compileNode(expr->expression, out);
};

int Compiler::compileIfStatement(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    IfStatement* ifStmt    = dynamic_cast<IfStatement*>(node);
    uint32_t     begOffset = out->getSize();

    Opcode ifBegin   = { OutputCodes::IF, 0x00 };
    Opcode elifBegin = { OutputCodes::ELIF, 0x00 };
    Opcode elseBegin = { OutputCodes::ELSE, 0x02 };
    Opcode endif     = { OutputCodes::ENDIF, 0x00 };

    int      exprLen, bodyLen;
    uint16_t exprLenOut, compLenOut, jumpOps;
    uint32_t exprLenOffset, jumpOpsOffset, compLenOffset;

    out->writeOpcode(ifBegin);

    compLenOut = 0;
    exprLenOut = 0;
    jumpOps    = dynamic_cast<StatementBlock*>(ifStmt->body)->nodes->size();

    compLenOffset = out->getSize() - sizeof(ifBegin.length);

    out->writeShort(jumpOps);

    exprLenOffset = out->getSize();
    out->writeShort(exprLenOut);

    exprLen = compileNode(ifStmt->condition, out);
    if (exprLen < 0) {
        return -1;
    }
    exprLenOut = (uint16_t)exprLen;

    bodyLen = compileNode(ifStmt->body, out);
    if (bodyLen < 0) {
        return -1;
    }

    //    jumpOps    = bodyLen;
    compLenOut = exprLenOut + sizeof(uint16_t) + sizeof(uint16_t);

    out->writeAt(compLenOffset, (uint8_t*)&compLenOut, sizeof(uint16_t));
    //out->writeAt(jumpOpsOffset, (uint8_t*)&jumpOps, sizeof(uint16_t));
    out->writeAt(exprLenOffset, (uint8_t*)&exprLenOut, sizeof(uint16_t));

    uint32_t elifsSize = ifStmt->elseIfs.size();

    if (elifsSize) {
        for (uint32_t i = 0; i < elifsSize; i++) {
            out->writeOpcode(elifBegin);

            compLenOut = 0;
            exprLenOut = 0;
            jumpOps    = dynamic_cast<StatementBlock*>(ifStmt->elseIfs[i].second)->nodes->size();

            compLenOffset = out->getSize() - sizeof(elifBegin.length);

            out->writeShort(jumpOps);

            exprLenOffset = out->getSize();
            out->writeShort(exprLenOut);

            exprLen = compileNode(ifStmt->elseIfs[i].first, out);
            if (exprLen < 0) {
                return -1;
            }
            exprLenOut = (uint16_t)exprLen;

            bodyLen = compileNode(ifStmt->elseIfs[i].second, out);
            if (bodyLen < 0) {
                return -1;
            }

            compLenOut = exprLenOut + sizeof(uint16_t) + sizeof(uint16_t);

            out->writeAt(compLenOffset, (uint8_t*)&compLenOut, sizeof(uint16_t));
            out->writeAt(exprLenOffset, (uint8_t*)&exprLenOut, sizeof(uint16_t));
        }
    }

    if (ifStmt->elseBody != nullptr) {
        out->writeOpcode(elseBegin);

        jumpOpsOffset = out->getSize();
        out->writeShort(jumpOps);

        bodyLen = compileNode(ifStmt->elseBody, out);

        if (bodyLen < 0) {
            return -1;
        }

        jumpOps = bodyLen;
        out->writeAt(jumpOpsOffset, (uint8_t*)&jumpOps, sizeof(uint16_t));
    }

    out->writeOpcode(endif);

    return out->getSize() - begOffset;
};

int Compiler::compileReferenceAccess(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    ReferenceAccess* refAccess = dynamic_cast<ReferenceAccess*>(node);
    uint32_t         begOffset = out->getSize();

    if (refAccess->context == NodeContext::Expression) {
        out->writeByte(ExprCodes::PUSH);
        out->writeByte(ExprCodes::REF_FUNC_PARAM);
    } else if (refAccess->context == NodeContext::Assignment) {
        out->writeByte(ExprCodes::REF_FUNC_PARAM);
    } else {
        out->writeByte(OutputCodes::REF_ACCESS);
    }

    uint16_t index = ctx.SCROLookup(refAccess->reference);
    out->writeShort(index);

    std::pair<VariableInfo, bool> retPair = this->ctx.getScriptLocalVar(refAccess->reference, refAccess->field);

    if (!retPair.second) {
        return -1;
    }

    uint8_t targetVarTypeCode;

    switch (retPair.first.type) {
    case (Type::Reference):
    case (Type::Float):
        targetVarTypeCode = ExprCodes::FLOAT_REF_LOCAL;
        break;
    case (Type::Short):
    case (Type::Integer):
        targetVarTypeCode = ExprCodes::INT_LOCAL;
        break;
    default:
        return -1;
        break;
    }

    out->writeByte(targetVarTypeCode);
    out->writeShort(retPair.first.index);

    return out->getSize() - begOffset;
}

int Compiler::compileVariable(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    Variable* var = dynamic_cast<Variable*>(node);

    if (var->variableType != Type::Reference) {

        if (!ctx.varExists(var->variableName))
            this->ctx.declareVar(var->variableName, var->variableType, VariableScope::Local);
    } else {
        if (!ctx.refVarExists(var->variableName))
            this->ctx.declareRefVar(var->variableName, VariableScope::Local);
    }

    return 0;
};

int Compiler::compileVariableAccess(Node* node, CompiledScript* out)
{

    VariableAccess* var     = dynamic_cast<VariableAccess*>(node);
    uint32_t        begSize = out->getSize();

    uint8_t  typeCode;
    uint16_t varIndex;

    if (var->context == NodeContext::Expression)
        out->writeByte(ExprCodes::PUSH);

    std::pair<VariableInfo, bool> varInfo = ctx.getVar(var->variableName);
    varIndex                              = varInfo.first.index;

    switch (varInfo.first.type) {
    case (Type::Reference):
    case (Type::Float):
        typeCode = ExprCodes::FLOAT_REF_LOCAL;
        break;
    case (Type::Short):
    case (Type::Integer):
        typeCode = ExprCodes::INT_LOCAL;
        break;
    default:
        return -1;
        break;
    }

    out->writeByte(typeCode);
    out->writeShort(varIndex);

    return out->getSize() - begSize;
}

int Compiler::compileStatementBlock(Node* node, CompiledScript* out)
{
    StatementBlock* block = dynamic_cast<StatementBlock*>(node);

    uint32_t begSize = out->getSize();
    uint32_t n       = block->nodes->size();
    int      err;

    for (uint32_t i = 0; i < n; i++) {
        err = compileNode(block->nodes->at(i), out);
        if (err < 0) {
            return -1;
        }
    }

    return out->getSize() - begSize;
}

int Compiler::compileReturnStatement(Node* node, CompiledScript* out)
{
    Opcode retcode = { 0x1E, 0x00 };

    out->writeOpcode(retcode);

    return sizeof(retcode);
}
};

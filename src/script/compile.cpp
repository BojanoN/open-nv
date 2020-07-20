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
            printf("\n%.5d: ", i + 1);
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

int compileBlocktype(BlockTypeStatement* node, CompiledScript* out)
{

    switch (node->blocktype) {
    default:
        log_fatal("Unknown blocktype %s", BlockTypeEnumToString(node->blocktype));
        return -1;
    }
    return 0;
}

int Compiler::compileScriptBlock(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    ScriptBlock* block = dynamic_cast<ScriptBlock*>(node);
    int          size  = block->nodes->size();

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

    uint8_t set[]                = { static_cast<uint8_t>(OutputCodes::ASSIGN), 0x00 };
    uint8_t exprLenPlaceholder[] = { 0x00, 0x00 };

    out->write(set, 2);
    uint16_t length = 0;
    uint32_t lengthOffset;

    lengthOffset = out->getSize();
    out->write((uint8_t*)&length, sizeof(uint16_t));

    int varLen = compileNode(expr->variable, out);
    if (varLen < 0) {
        return -1;
    }

    uint32_t exprLenOffset = out->getSize();
    out->write(exprLenPlaceholder, 2);

    int exprLen = compileNode(expr->expression, out);
    if (exprLen < 0) {
        return -1;
    }
    uint16_t exprLenOut = (uint16_t)exprLen;
    out->writeAt(exprLenOffset, (uint8_t*)&exprLenOut, sizeof(exprLenOut));

    length = varLen + sizeof(uint16_t) + exprLenOut;
    out->writeAt(lengthOffset, (uint8_t*)&length, sizeof(length));

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
            // TODO: SCRO lookup

            typeCode          = static_cast<uint8_t>(ExprCodes::REF_GLOBAL);
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

int Compiler::compileFunctionCall(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    FunctionCall* func    = dynamic_cast<FunctionCall*>(node);
    uint32_t      begSize = out->getSize();

    FunctionInfo& info = FunctionResolver::getFunctionInfo(func->functionName);

    // Reference function call, inside an expression
    // TODO: check the esm file for confirmation
    if (func->reference.size()) {
        out->writeByte(ExprCodes::REF_FUNC_PARAM);
        uint16_t index = ctx.SCROLookup(func->reference);
        out->write((uint8_t*)&index, sizeof(uint16_t));
    }

    if (func->context == NodeContext::Expression) {
        out->writeByte(ExprCodes::REF_FUNC_PARAM);
    }

    uint16_t funcCode = info.opcode;
    out->write((uint8_t*)&funcCode, sizeof(uint16_t));

    uint32_t paramBytesOffset = out->getSize();
    out->writeZeros(sizeof(uint16_t));

    uint16_t paramCount = func->arguments.size();
    out->write((uint8_t*)&paramCount, sizeof(uint16_t));

    uint16_t     paramBytes = 0;
    LiteralExpr* literal;

    for (uint32_t i = 0; i < paramCount; i++) {
        // TODO: add information about the number of parameters required for each available function
        // so we can enable nested function calls
        if (func->arguments[i]->type != NodeType::LiteralExpr) {
            log_fatal("Function arguments can only be constant values");
            return -1;
        }

        literal = dynamic_cast<LiteralExpr*>(func->arguments[i]);
        log_debug("WHAT: %s", TypeEnumToString(literal->valueType));
        switch (literal->valueType) {
        case (Type::Integer): {
            out->writeByte(ExprCodes::LONG_FUNC_PARAM);
            uint32_t value = std::stol(literal->value);
            out->write((uint8_t*)&value, sizeof(uint32_t));
            paramBytes += 1 + sizeof(uint32_t);
            break;
        }
        case (Type::Reference):
        case (Type::Identifier): {
            out->writeByte(ExprCodes::REF_FUNC_PARAM);
            uint16_t index = ctx.SCROLookup(literal->value);
            out->write((uint8_t*)&index, sizeof(uint16_t));
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

    return out->getSize() - begSize;
};

int Compiler::compileScriptName(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    ScriptNameStatement* scriptName = dynamic_cast<ScriptNameStatement*>(node);

    this->ctx.scriptName = scriptName->name;

    uint8_t scn[] = { 0x1D, 0x00, 0x00, 0x00 };

    out->write(scn, sizeof(scn));

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

    uint8_t ifBegin[]   = { static_cast<uint8_t>(OutputCodes::IF), 0x00 };
    uint8_t elifBegin[] = { static_cast<uint8_t>(OutputCodes::ELIF), 0x00 };
    uint8_t elseBegin[] = { static_cast<uint8_t>(OutputCodes::ELSE), 0x00, 0x02, 0x00 };

    int      exprLen, compLen, bodyLen;
    uint16_t exprLenOut, compLenOut, jumpOps;
    uint32_t exprLenOffset, jumpOpsOffset, compLenOffset;

    out->write(ifBegin, 2);

    // TODO: check .esm files for source and see what this is supposed to be
    compLenOut = 0;
    jumpOps    = 0;
    exprLenOut = 0;

    compLenOffset = out->getSize();
    out->write((uint8_t*)&compLenOut, sizeof(uint16_t));

    jumpOpsOffset = out->getSize();
    out->write((uint8_t*)&jumpOps, sizeof(uint16_t));

    exprLenOffset = out->getSize();
    out->write((uint8_t*)&exprLenOut, sizeof(uint16_t));

    exprLen = compileNode(ifStmt->condition, out);
    if (exprLen < 0) {
        return -1;
    }
    exprLenOut = (uint16_t)exprLen;

    bodyLen = compileNode(ifStmt->body, out);
    if (bodyLen < 0) {
        return -1;
    }

    jumpOps    = bodyLen;
    compLenOut = exprLenOut + sizeof(uint16_t);

    out->writeAt(compLenOffset, (uint8_t*)&compLenOut, sizeof(uint16_t));
    out->writeAt(jumpOpsOffset, (uint8_t*)&jumpOps, sizeof(uint16_t));
    out->writeAt(exprLenOffset, (uint8_t*)&exprLenOut, sizeof(uint16_t));

    uint32_t elifsSize = ifStmt->elseIfs.size();

    if (elifsSize) {
        for (uint32_t i = 0; i < elifsSize; i++) {
            out->write(elifBegin, 2);

            compLenOut = 0;
            jumpOps    = 0;
            exprLenOut = 0;

            compLenOffset = out->getSize();
            out->write((uint8_t*)&compLenOut, sizeof(uint16_t));

            jumpOpsOffset = out->getSize();
            out->write((uint8_t*)&jumpOps, sizeof(uint16_t));

            exprLenOffset = out->getSize();
            out->write((uint8_t*)&exprLenOut, sizeof(uint16_t));

            exprLen = compileNode(ifStmt->elseIfs[i].first, out);
            if (exprLen < 0) {
                return -1;
            }
            exprLenOut = (uint16_t)exprLen;

            bodyLen = compileNode(ifStmt->elseIfs[i].second, out);
            if (bodyLen < 0) {
                return -1;
            }

            jumpOps    = bodyLen;
            compLenOut = exprLenOut + sizeof(uint16_t);

            out->writeAt(compLenOffset, (uint8_t*)&compLenOut, sizeof(uint16_t));
            out->writeAt(jumpOpsOffset, (uint8_t*)&jumpOps, sizeof(uint16_t));
            out->writeAt(exprLenOffset, (uint8_t*)&exprLenOut, sizeof(uint16_t));
        }
    }

    if (ifStmt->elseBody != nullptr) {
        out->write(elseBegin, 4);
        jumpOpsOffset = out->getSize();
        out->write((uint8_t*)&jumpOps, sizeof(uint16_t));

        bodyLen = compileNode(ifStmt->elseBody, out);

        if (bodyLen < 0) {
            return -1;
        }

        jumpOps = bodyLen;
        out->writeAt(jumpOpsOffset, (uint8_t*)&jumpOps, sizeof(uint16_t));
    }

    return out->getSize() - begOffset;
};

int Compiler::compileReferenceAccess(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    ReferenceAccess* refAccess = dynamic_cast<ReferenceAccess*>(node);
    uint32_t         begOffset = out->getSize();

    out->writeByte((refAccess->context == NodeContext::Expression) ? ExprCodes::REF_FUNC_PARAM : OutputCodes::REF_ACCESS);
    uint16_t index = ctx.SCROLookup(refAccess->reference);
    out->write((uint8_t*)&index, sizeof(uint16_t));

    return out->getSize() - begOffset;
}

int Compiler::compileVariable(Node* node, CompiledScript* out)
{
    CHECK_NULL(node);

    Variable* var = dynamic_cast<Variable*>(node);

    if (!ctx.varExists(var->variableName))
        this->ctx.declareVar(var->variableName, var->variableType);

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

    out->writeByte(typeCode);
    out->write((uint8_t*)&varIndex, sizeof(uint16_t));

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
    ReturnStatement* retStmt = dynamic_cast<ReturnStatement*>(node);

    uint8_t retcode[4] = { 0x1E, 0, 0, 0 };

    out->write(retcode, sizeof(retcode));

    return sizeof(retcode);
}
};

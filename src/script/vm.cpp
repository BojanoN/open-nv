#include "vm.hpp"
#include "logc/log.h"

namespace Script {
bool VM::execute(CompiledScript* script)
{
    bool err;
    this->script = script;

    // Check for beginning header
    uint16_t opcode = script->readShort();
    if (opcode != OutputCodes::SCRIPTNAME) {
        log_fatal("Invalid script name opcode");
        this->script = nullptr;
        return true;
    }

    opcode = script->readShort();
    if (opcode != 0) {
        log_fatal("Invalid script name opcode");
        this->script = nullptr;
        return true;
    }

    // Blocktype
    // TODO: only GameMode supported for now, add other blocktypes
    opcode = script->readShort();
    if (opcode != OutputCodes::BEGIN) {
        log_fatal("Invalid script beginning opcode");
        this->script = nullptr;
        return true;
    }

    opcode = script->readShort();
    if (opcode != 0x6) {
        log_fatal("Invalid script beginning opcode");
        this->script = nullptr;
        return true;
    }

    opcode = script->readShort();
    if (opcode != 0) {
        log_fatal("Invalid script beginning opcode");
        this->script = nullptr;
        return true;
    }

    script->readLong();

    while (script->hasMoreBytes()) {

        err = handleOpcode();

        if (err) {
            log_fatal("VM execution error at offset: %d", script->getReadOffset());
            this->script = nullptr;
            return true;
        }
    }

    this->script = nullptr;

    return false;
};

bool VM::numberParse()
{
    bool isFloat = false;
    // TODO: refactor this, add bound checking to avoid nasty buffer overflows
    char     tmpbuf[4096] = { 0 };
    uint32_t i            = 0;

    char c;
    while (true) {
        c = script->peekByte();

        if (!(c >= '0' && c <= '9'))
            break;

        tmpbuf[i] = script->readByte();
        i++;
    }

    if (script->peekByte() == '.') {
        isFloat = true;

        c         = script->readByte();
        tmpbuf[i] = c;
        i++;

        c = script->peekByte();

        while (true) {
            c = script->peekByte();

            if (!(c >= '0' && c <= '9'))
                break;

            tmpbuf[i] = script->readByte();
            i++;
        }
    }

    tmpbuf[i] = 0;

    try {
        Value val;
        val.type = isFloat ? Type::Float : Type::Integer;

        if (isFloat) {
            val.value.f = std::stof(tmpbuf);
        } else {
            val.value.l = std::stod(tmpbuf);
        }

        log_debug("Number parse result: %s, %d", TypeEnumToString(val.type), AS_INT(val));

        this->stack.push(val);
    } catch (std::invalid_argument& e) {
        log_fatal("numberParse failure: %s", e.what());
        return true;
    }
    return false;
}

bool VM::functionCall()
{

    return false;
};

enum TwoCharOperators : uint16_t {
    EQUAL     = 0x3d3d,
    NOT_EQUAL = 0x3d21,
    GTE       = 0x3d3e,
    LTE       = 0x3d3c
};

static std::set<uint8_t>  singleCharOp = { '+', '-', '*', '/' };
static std::set<uint16_t> comparisonOp = {
    TwoCharOperators::EQUAL,
    TwoCharOperators::NOT_EQUAL,
    TwoCharOperators::GTE,
    TwoCharOperators::LTE
};

bool VM::handleBinaryOperator()
{
    uint8_t  code = script->peekByte();
    uint16_t comp = script->peekShort();
    Value    res;

    if (singleCharOp.count(code)) {
        script->readByte();
        Value right = this->stack.pop();
        Value left  = this->stack.pop();

        if (right.type == Type::Reference || left.type == Type::Reference) {
            log_fatal("Reference arithmetic is not allowed");
            return true;
        }

        switch (code) {
        case ('+'):
            ARITHMETIC_OP(left, right, +);
            break;
        case ('-'):
            ARITHMETIC_OP(left, right, -);
            break;
        case ('*'):
            ARITHMETIC_OP(left, right, *);
            break;
        case ('/'):
            ARITHMETIC_OP(left, right, /);
            break;
        case ('>'):
            COMPARISON_OP(left, right, >);
            break;
        case ('<'):
            COMPARISON_OP(left, right, <);
            break;
        default:
            break;
        }

        return false;
    } else if (comparisonOp.count(comp)) {
        script->readShort();

        Value right = this->stack.pop();
        Value left  = this->stack.pop();

        switch (comp) {
        case (TwoCharOperators::EQUAL):
            COMPARISON_OP(left, right, ==);
            break;
        case (TwoCharOperators::NOT_EQUAL):
            COMPARISON_OP(left, right, !=);
            break;
        case (TwoCharOperators::GTE):
            COMPARISON_OP(left, right, >=);
            break;
        case (TwoCharOperators::LTE):
            COMPARISON_OP(left, right, <=);
            break;
        default:
            break;
        }

        return false;
    }

    return true;
}

bool VM::handleExpressionCode()
{
    uint8_t code = script->peekByte();
    bool    err  = false;

    switch (code) {
    case (ExprCodes::FUNC):
        script->readByte();
        return functionCall();
    case (ExprCodes::INT_LOCAL):
    case (ExprCodes::FLOAT_REF_LOCAL): {
        script->readByte();
        uint16_t index = script->readShort();
        this->stack.push(script->getLocalVariable(index));
        return false;
    }
    case (ExprCodes::REF_GLOBAL): {
        script->readByte();
        uint16_t index = script->readShort();
        this->stack.push(script->getLocalVariable(index));
        return false;
        // TODO: SCRO/SCRV lookup
    }
    case (ExprCodes::GLOBAL):
        // TODO: SCRO/SCRV lookup
        break;
        /*
    case (ExprCodes::REF_FUNC):
    break;
        */
    default:
        break;
    }

    // This is a really terrible way to do this but bethesda's poor bytecode design has forced me
    // Two character operator parsing
    err = handleBinaryOperator();

    if (!err)
        return false;
    // Constants parsing
    return numberParse();
};

bool VM::executeBlock(uint16_t blocklen)
{
    uint32_t blockEnd = script->getReadOffset() + blocklen;
    bool     err;

    while (this->script->isBeforeOffset(blockEnd)) {
        err = handleOpcode();
        if (err) {
            return true;
        }
    }

    return false;
}

bool VM::handleIf()
{
    uint16_t compLen, jumpOps, exprLen, blockLen;
    uint16_t nextOpcode;
    bool     err;
    bool     evalResult;

    // If block
    compLen = script->readShort();
    jumpOps = script->readShort();
    exprLen = script->readShort();

    err = evalExpression(exprLen);

    if (err) {
        return true;
    }

    Value conditionResult = this->stack.pop();

    evalResult = AS_BOOL(conditionResult);

    if (evalResult) {
        log_debug("If expression evaluated to true");
        // TODO: find a sane way of skipping through all other bytecode related to the statement
        // which wont be executed
        blockLen = jumpOps;
        err      = executeBlock(blockLen);
        if (err) {
            return true;
        }

        goto skip;
    }

    script->jump(jumpOps);
    // if else
    nextOpcode = script->peekShort();

    while (nextOpcode == (uint8_t)OutputCodes::ELIF) {
        script->readShort();
        compLen = script->readShort();
        jumpOps = script->readShort();
        exprLen = script->readShort();

        err = evalExpression(exprLen);

        if (err) {
            return true;
        }

        Value conditionResult = this->stack.pop();

        evalResult = AS_BOOL(conditionResult);

        if (evalResult) {
            log_debug("Elif expression evaluated to true");
            blockLen = jumpOps;

            err = executeBlock(blockLen);
            if (err) {
                return true;
            }

            goto skip;
        }
        nextOpcode = script->peekShort();
    }

    if (nextOpcode == (uint8_t)OutputCodes::ELSE) {
        script->readShort();
        jumpOps  = script->readShort();
        blockLen = jumpOps;

        return executeBlock(blockLen);
    }

skip:
    nextOpcode = script->peekShort();

    while (nextOpcode == (uint8_t)OutputCodes::ELIF) {
        script->readShort();
        compLen = script->readShort();
        jumpOps = script->readShort();
        exprLen = script->readShort();

        script->jump(jumpOps);
        nextOpcode = script->peekShort();
    }

    if (nextOpcode == (uint8_t)OutputCodes::ELSE) {
        script->readShort();
        jumpOps = script->readShort();
        script->jump(jumpOps);
    }

    return false;
}

bool VM::handleAssign()
{
    uint16_t    varnameLen = script->readShort();
    std::string var;
    bool        err;
    log_debug("SIZE: 0x%x", varnameLen);
    var.reserve(varnameLen + 1);

    if (!script->readString(varnameLen, var.data())) {
        log_fatal("Unexpected end of script");
        return true;
    }
    var[varnameLen] = 0;

    log_debug("ASSIGN: %s", var.c_str());

    uint16_t exprLen = script->readShort();
    err              = evalExpression(exprLen);
    if (err) {
        return true;
    }

    Value exprRes = this->stack.pop();

    log_debug("ASSIGN: Eval result: %s, %d", TypeEnumToString(exprRes.type), AS_INT(exprRes));

    // script->context.assign()

    return false;
};

bool VM::evalExpression(uint16_t exprLen)
{
    uint32_t exprEnd = script->getReadOffset() + exprLen;
    uint8_t  currentOpcode;
    bool     err;

    while (script->isBeforeOffset(exprEnd)) {
        currentOpcode = script->readByte();
        if (currentOpcode != ExprCodes::PUSH) {
            log_fatal("Expected push, got 0x%x", currentOpcode);
            return true;
        }
        log_debug("BENIS: 0x%x", script->peekByte());

        err = handleExpressionCode();
        if (err) {
            return true;
        }
    }

    return false;
}

bool VM::handleOpcode()
{
    uint16_t opcode = script->readShort();
    bool     err    = false;

    switch (opcode) {
    case (OutputCodes::IF):
        return handleIf();
    case (OutputCodes::ASSIGN):
        return handleAssign();
    default:
        log_fatal("Unknown opcode 0x%x", opcode);
        err = true;
        break;
    }

    return err;
};
};

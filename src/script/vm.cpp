#include "vm.hpp"
#include "function.hpp"
#include "logc/log.h"

namespace Script {
bool VM::execute(CompiledScript* script)
{
    VMStatusCode err;
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

        switch (err) {
        case VMStatusCode::VM_OK:
            break;
        case VMStatusCode::VM_END:
            this->script = nullptr;
            return false;
        default:
            log_fatal("VM execution error at offset: %d", script->getReadOffset());
            log_fatal("VM status code: %s", VMStatusCodeToString(err));
            this->script = nullptr;
            return true;
        }
    }

    this->script = nullptr;

    return false;
};

VMStatusCode VM::numberParse()
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
        return VMStatusCode::VM_GENERIC_ERROR;
    }
    return VMStatusCode::VM_OK;
}

VMStatusCode VM::functionCall(uint16_t opcode, int16_t refIndex)
{

    if (!FunctionResolver::lookupFunction(opcode)) {
        return VMStatusCode::VM_UNKNOWN_FUNC_OPCODE;
    }

    uint16_t paramBytes = script->readShort();
    uint16_t paramEnd   = script->getReadOffset() + paramBytes;
    //uint16_t paramCount = script->readShort();

    uint8_t  paramOpcode;
    uint32_t noPushed = 0;

    if (refIndex) {
        // TODO: check if the target reference supports the following function call
    }

    while (script->isBeforeOffset(paramEnd)) {
        Value val;
        paramOpcode = script->readByte();

        switch (paramOpcode) {
        case (ExprCodes::REF_FUNC_PARAM): {
            val.type = Type::Reference;
            // TODO: we are actually reading the SCRO index here.
            // Replace with the actual reference once things are setup.
            val.value.l = script->readShort();
            this->stack.push(val);
            break;
        }
        case (ExprCodes::LONG_FUNC_PARAM): {
            val.type    = Type::Integer;
            val.value.l = script->readLong();
            this->stack.push(val);
            break;
        }
        case (ExprCodes::FLOAT_FUNC_PARAM): {
            val.type    = Type::Float;
            val.value.f = script->readDouble();
            this->stack.push(val);
            break;
        }
        default:
            log_fatal("Unknown function parameter opcode!");
            return VMStatusCode::VM_UNKNOWN_FUNC_PARAM;
        }
        noPushed++;
    }

    if (!FunctionResolver::callFunction(opcode)) {
        return VMStatusCode::VM_FUNC_CALL_FAILED;
    }

    // Function calls should clean the stack, but for now we clean it manually
    for (uint16_t i = 0; i < noPushed; i++) {
        Value tmp = this->stack.pop();

#ifdef DEBUG
        log_debug("Function argument: %s, %d", TypeEnumToString(tmp.type), AS_INT(tmp));
#endif
    }

    return VMStatusCode::VM_OK;
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

VMStatusCode VM::handleBinaryOperator()
{
    uint8_t  code = script->peekByte();
    uint16_t comp = script->peekShort();
    //Value    res;

    if (singleCharOp.count(code)) {
        script->readByte();
        Value right = this->stack.pop();
        Value left  = this->stack.pop();

        if (right.type == Type::Reference || left.type == Type::Reference) {
            log_fatal("Reference arithmetic is not allowed");
            return VMStatusCode::VM_REFERENCE_ARITHMETIC;
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

        return VMStatusCode::VM_OK;
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

        return VM_OK;
    }

    return VM_GENERIC_ERROR;
}

VMStatusCode VM::handleReferenceAccess()
{

    uint16_t index = script->readShort();

    // Only function calls are handled for now
    uint16_t functionID = script->readShort();

    return functionCall(functionID, index);
}

VMStatusCode VM::handleExpressionCode()
{
    uint8_t      code = script->peekByte();
    VMStatusCode err;

    switch (code) {
    case (ExprCodes::INT_LOCAL):
    case (ExprCodes::FLOAT_REF_LOCAL): {
        script->readByte();
        uint16_t index = script->readShort();
        this->stack.push(script->getLocalVariable(index));
        return VMStatusCode::VM_OK;
    }
    case (ExprCodes::REF_GLOBAL): {
        script->readByte();
        uint16_t index = script->readShort();
        this->stack.push(script->getLocalVariable(index));
        return VMStatusCode::VM_OK;
        // TODO: SCRO/SCRV lookup
    }
    case (ExprCodes::GLOBAL):
        // TODO: SCRO/SCRV lookup
        break;
    case (ExprCodes::REF_FUNC_PARAM):
        script->readByte();
        return handleReferenceAccess();
    default:
        break;
    }

    // This is a really terrible way to do this but bethesda's poor bytecode design has forced me
    // Two character operator parsing
    err = handleBinaryOperator();

    if (!err)
        return VMStatusCode::VM_OK;
    // Constants parsing
    return numberParse();
};

VMStatusCode VM::executeBlock(uint16_t blocklen)
{
    uint32_t     blockEnd = script->getReadOffset() + blocklen;
    VMStatusCode err;

    while (this->script->isBeforeOffset(blockEnd)) {
        err = handleOpcode();
        if (err) {
            return err;
        }
    }

    return VMStatusCode::VM_OK;
}

VMStatusCode VM::handleIf()
{
    uint16_t     compLen, jumpOps, exprLen, blockLen;
    uint16_t     nextOpcode;
    VMStatusCode err;
    bool         evalResult = false;

    // If block
    compLen = script->readShort();
    jumpOps = script->readShort();
    exprLen = script->readShort();

    err = evalExpression(exprLen);

    if (err) {
        return err;
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
            return err;
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
            return err;
        }

        Value conditionResult = this->stack.pop();

        evalResult = AS_BOOL(conditionResult);

        if (evalResult) {
            log_debug("Elif expression evaluated to true");
            blockLen = jumpOps;

            err = executeBlock(blockLen);
            if (err) {
                return err;
            }

            goto skip;
        }
        nextOpcode = script->peekShort();
    }

    if (nextOpcode == (uint8_t)OutputCodes::ELSE) {
        script->readShort();
        compLen  = script->readShort();
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

        script->jump(jumpOps + exprLen);
        nextOpcode = script->peekShort();
    }

    if (nextOpcode == (uint8_t)OutputCodes::ELSE) {
        script->readShort();
        compLen = script->readShort();
        jumpOps = script->readShort();
        script->jump(jumpOps);
    }

    return VMStatusCode::VM_OK;
}

VMStatusCode VM::handleAssign()
{
    uint16_t     varnameLen = script->readShort();
    std::string  var;
    VMStatusCode err;
    log_debug("SIZE: 0x%x", varnameLen);
    var.reserve(varnameLen + 1);

    if (!script->readString(varnameLen, (char*)var.data())) {
        log_fatal("Unexpected end of script");
        return VMStatusCode::VM_UNEXP_EOF;
    }
    var[varnameLen] = 0;

    log_debug("ASSIGN: %s", var.c_str());

    uint16_t exprLen = script->readShort();
    err              = evalExpression(exprLen);
    if (err) {
        return err;
    }

    Value exprRes = this->stack.pop();

    log_debug("ASSIGN: Eval result: %s, %d", TypeEnumToString(exprRes.type), AS_INT(exprRes));

    // script->context.assign()

    return VMStatusCode::VM_OK;
};

VMStatusCode VM::evalExpression(uint16_t exprLen)
{
    uint32_t     exprEnd = script->getReadOffset() + exprLen;
    uint8_t      currentOpcode;
    VMStatusCode err;

    while (script->isBeforeOffset(exprEnd)) {
        currentOpcode = script->readByte();
        if (currentOpcode != ExprCodes::PUSH) {
            log_fatal("Expected push, got 0x%x", currentOpcode);
            return VMStatusCode::VM_EVAL_FAILED;
        }
        log_debug("BENIS: 0x%x", script->peekByte());

        err = handleExpressionCode();
        if (err) {
            return err;
        }
    }

    return VMStatusCode::VM_OK;
}

VMStatusCode VM::handleOpcode()
{
    // Reference function call
    if (script->peekByte() == ExprCodes::REF_FUNC_PARAM) {
        script->readByte();
        return handleReferenceAccess();
    }

    uint16_t     opcode = script->readShort();
    VMStatusCode err    = VMStatusCode::VM_OK;

    switch (opcode) {
    case (OutputCodes::IF):
        return handleIf();
    case (OutputCodes::ASSIGN):
        return handleAssign();
    case (OutputCodes::END):
        // consume two trailing zeros
        script->readShort();
        return VMStatusCode::VM_END;

    default: {
        err = functionCall(opcode, -1);
        break;
    }
    }

    return err;
};
};

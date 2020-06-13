#include "vm.hpp"
#include "logc/log.h"

namespace Script {
bool VM::execute(CompiledScript* script)
{
    bool err;
    this->script = script;

    while (script->hasMoreBytes()) {

        err = handleOpcode();

        if (err) {
            log_fatal("VM execution error");
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
    char     tmpbuf[512];
    uint32_t i = 0;

    char c = script->peekByte();
    while (c >= '0' && c <= '9') {
        tmpbuf[i] = script->readByte();
        i++;
    }

    if (script->peekByte() == '.') {
        isFloat = true;

        tmpbuf[i] = script->readByte();
        i++;

        while (c >= '0' && c <= '9') {
            tmpbuf[i] = script->readByte();
            i++;
        }
    }

    tmpbuf[i + 1] = 0;

    try {
        Value val;
        val.type = isFloat ? Type::Float : Type::Integer;

        if (isFloat) {
            val.value.f = std::stof(tmpbuf);
        } else {
            val.value.l = std::stod(tmpbuf);
        }

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
    case ('+'):
    case ('-'):
    case ('*'):
    case ('/'):

    case (ExprCodes::REF_GLOBAL):
        // TODO: SCRO/SCRV lookup
        break;
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

    // Why would anyone do this
    // Two character operator parsing
    uint16_t op = script->peekShort();
    switch (op) {
    };
    // Constants parsing
    return numberParse();
};

bool VM::evalExpression(uint16_t exprLen)
{
    uint32_t exprEnd = script->getReadOffset() + exprLen;
    uint8_t  currentOpcode;

    while (script->isBeforeOffset(exprEnd)) {
        currentOpcode = script->readByte();
        if (currentOpcode != ExprCodes::PUSH) {
            log_fatal("Expected push");
            return true;
        }
    }

    return false;
}

bool VM::handleOpcode()
{
    uint16_t opcode = script->peekShort();
    bool     err    = false;

    switch (opcode) {

    default:
        log_fatal("Unknown opcode");
        err = true;
        break;
    }

    return err;
};
};

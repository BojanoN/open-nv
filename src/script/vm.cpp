#include "vm.hpp"
#include "logc/log.h"

namespace Script {
bool VM::execute(CompiledScript* script)
{
    bool err;
    while (script->hasMoreBytes()) {
        err = handleExpressionCode(script);

        if (!err) {
            continue;
        }

        err = handleOpcode(script);

        if (err) {
            log_fatal("VM execution error");
            return true;
        }
    }

    return false;
};

bool VM::handleExpressionCode(CompiledScript* script)
{
    uint8_t code = script->peekByte();
    bool    err  = false;

    switch (code) {
    default:
        err = true;
        break;
    }

    return err;
};

bool VM::handleOpcode(CompiledScript* script)
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

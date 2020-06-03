#pragma once

#include <cstdint>

namespace Script {

#define OPCODE_ENUM      \
    X(SCRIPTNAME, 0x1D)  \
    X(NOP, 0x00)         \
    X(HALT, 0x01)        \
    X(JMP, 0x02)         \
    X(JMP_IF_TRUE, 0x03) \
    X(POP, 0x04)         \
    X(PUSH, 0x05)        \
    X(FUNC_CALL, 0x06)   \
    X(LOOKUP, 0x07)      \
    X(STORE, 0x08)       \
    X(CONSTANT, 0x09)    \
    X(CMP, 0x0A)         \
    X(ADD, 0x0B)         \
    X(SUB, 0x0C)         \
    X(MUL, 0x0D)         \
    X(DIV, 0x0E)

#define X(name, value) name = value,

enum class OpcodeType : uint16_t {
    OPCODE_ENUM
};

#undef X

constexpr const char* opcodeTypeToString(OpcodeType e) noexcept
{
#define X(name, value)       \
    case (OpcodeType::name): \
        return #name;

    switch (e) {
        OPCODE_ENUM
    }
#undef X
}

struct Opcode {
    OpcodeType bytecode;
    uint16_t   arglen;
};
};

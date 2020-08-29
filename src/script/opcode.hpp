#pragma once

#include <cstdint>

// TODO: refactor opcode definition with a separate struct

namespace Script {

#define OUTPUT_CODES_ENUM \
    X(SCRIPTNAME, 0x1D)   \
    X(BEGIN, 0x10)        \
    X(END, 0x11)          \
    X(RET, 0x1E)          \
    X(IF, 0x16)           \
    X(ELSE, 0x17)         \
    X(ELIF, 0x18)         \
    X(ENDIF, 0x19)        \
    X(ASSIGN, 0x15)       \
    X(REF_ACCESS, 0x1C)   \
    X(SET_GLOBAL, 0x47)

#define X(name, value) name = value,

enum OutputCodes : uint16_t {
    OUTPUT_CODES_ENUM
};

#undef X

constexpr const char* outputCodeToString(OutputCodes e) noexcept
{
#define X(name, value)        \
    case (OutputCodes::name): \
        return #name;

    switch (e) {
        OUTPUT_CODES_ENUM
    }
#undef X
}

#define EXPR_CODES_ENUM       \
    X(INT_LOCAL, 0x73)        \
    X(FLOAT_REF_LOCAL, 0x66)  \
    X(GLOBAL, 0x47)           \
    X(REF_GLOBAL, 0x5A)       \
    X(REF_FUNC_PARAM, 0x72)   \
    X(LONG_FUNC_PARAM, 0x6E)  \
    X(FLOAT_FUNC_PARAM, 0x7A) \
    X(FUNC_CALL, 0x58)        \
    X(PUSH, 0x20)

#define X(name, value) name = value,

enum ExprCodes : uint8_t {
    EXPR_CODES_ENUM
};

#undef X

constexpr const char* exprCodeToString(ExprCodes e) noexcept
{
#define X(name, value)      \
    case (ExprCodes::name): \
        return #name;

    switch (e) {
        EXPR_CODES_ENUM
    }
#undef X
}

#define BLOCK_CODES_ENUM \
    X(GAMEMODE, 0x00)

#define X(name, value) name = value,

enum BlocktypeCodes : uint16_t {
    BLOCK_CODES_ENUM
};

#undef X

constexpr const char* BlocktypeCodeToString(BlocktypeCodes e) noexcept
{
#define X(name, value)           \
    case (BlocktypeCodes::name): \
        return #name;

    switch (e) {
        BLOCK_CODES_ENUM
    }
#undef X
}

struct Opcode {
    uint16_t code;
    uint16_t length;
};

};

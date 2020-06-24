#pragma once

#include <cstdint>

namespace Script {

#define OUTPUT_CODES_ENUM \
    X(SCRIPTNAME, 0x1D)   \
    X(BEGIN, 0x10)        \
    X(END, 0x11)          \
    X(RET, 0x1E)          \
    X(IF, 0x16)           \
    X(ELSE, 0x17)         \
    X(ELIF, 0x18)         \
    X(ASSIGN, 0x15)       \
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
    X(PUSH, 0x20)             \
    X(FUNC, 0x58)

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

};

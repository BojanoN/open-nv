#pragma once

#include <cstdint>

namespace Script {

#define OUTPUT_CODES_ENUM        \
    X(SCRIPTNAME, 0x1D)          \
    X(BEGIN, 0x10)               \
    X(END, 0x11)                 \
    X(RET, 0x1E)                 \
    X(IF, 0x16)                  \
    X(ELSE, 0x17)                \
    X(ELIF, 0x18)                \
    X(REF_FUNC_PARAM, 0x72)      \
    X(LONG_FUNC_PARAM, 0x6E)     \
    X(FLOAT_FUNC_PARAM, 0x7A)    \
    X(ASSIGN, 0x15)              \
    X(SET_INT_LOCAL, 0x73)       \
    X(SET_FLOAT_REF_LOCAL, 0x66) \
    X(SET_GLOBAL, 0x47)          \
    X(ADD, 0x0B)                 \
    X(SUB, 0x0C)                 \
    X(MUL, 0x0D)                 \
    X(DIV, 0x0E)                 \
    X(REF, 0x1C)                 \
    X(PUSH, 0x20)                \
    X(FUNC, 0x58)

#define X(name, value) name = value,

enum class OutputCodes : uint16_t {
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

#define EXPR_CODES_ENUM          \
    X(SCRIPTNAME, 0x1D)          \
    X(BEGIN, 0x10)               \
    X(END, 0x11)                 \
    X(RET, 0x1E)                 \
    X(IF, 0x16)                  \
    X(ELSE, 0x17)                \
    X(ELIF, 0x18)                \
    X(REF_FUNC_PARAM, 0x72)      \
    X(LONG_FUNC_PARAM, 0x6E)     \
    X(FLOAT_FUNC_PARAM, 0x7A)    \
    X(ASSIGN, 0x15)              \
    X(SET_INT_LOCAL, 0x73)       \
    X(SET_FLOAT_REF_LOCAL, 0x66) \
    X(SET_GLOBAL, 0x47)          \
    X(ADD, 0x0B)                 \
    X(SUB, 0x0C)                 \
    X(MUL, 0x0D)                 \
    X(DIV, 0x0E)                 \
    X(REF, 0x1C)                 \
    X(PUSH, 0x20)                \
    X(FUNC, 0x58)

#define X(name, value) name = value,

enum class ExprCodes : uint8_t {
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
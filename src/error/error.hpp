#pragma once
#include <cstdint>

namespace Err {

typedef uint32_t error_t;

#define ERROR_CODE_ENUM          \
    X(codeSuccess)               \
    X(codeInvalidArgument)       \
    X(codeKeyNotFound)           \
    X(codeTypeError)             \
    X(codeNullPointerError)      \
    X(codeNumberFormatError)     \
    X(codeOutOfRange)            \
    X(codeFloatInfinity)         \
    X(codeFloatNaN)              \
    X(codeMemoryError)           \
    X(codeResourceCreationError) \
    X(codeShaderCreationError)   \
    X(codeFileNotFound)          \
    X(codeInitError)             \
    X(codeEndOfOperation)        \
    X(codeContinue)

#define X(name) name,

enum ErrorCodes : error_t {
    ERROR_CODE_ENUM
};

#undef X

class Error {

    error_t value;

public:
    Error(error_t value)
        : value { value }
    {
    }

    Error()
        : value { Err::ErrorCodes::codeSuccess }
    {
    }

    bool fail() { return value != codeSuccess; }
    bool success() { return !fail(); }

    static constexpr const char* toString(ErrorCodes e)
    {
#define X(name)              \
    case (ErrorCodes::name): \
        return #name;

        switch (e) {
            ERROR_CODE_ENUM

        default:
            return "Unknown error code type";
        }
#undef X
    }
};

inline Error Success(codeSuccess);
inline Error InvalidArgument(codeInvalidArgument);
inline Error KeyNotFound(codeKeyNotFound);
inline Error TypeError(codeTypeError);
inline Error NullPointerError(codeNullPointerError);
inline Error NumberFormatError(codeNumberFormatError);
inline Error OutOfRange(codeOutOfRange);
inline Error FloatInfinity(codeFloatInfinity);
inline Error FloatNaN(codeFloatNaN);
inline Error MemoryError(codeMemoryError);
inline Error ResourceCreationError(codeResourceCreationError);
inline Error ShaderCreationError(codeShaderCreationError);
inline Error FileNotFound(codeFileNotFound);
inline Error InitError(codeInitError);
inline Error EndOfOperation(codeEndOfOperation);
inline Error Continue(codeContinue);
};

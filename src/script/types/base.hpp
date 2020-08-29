#pragma once

#include "../tokenizer.hpp"
#include <cstdint>

namespace Script {

#define TYPE_ENUM \
    X(Reference)  \
    X(Integer)    \
    X(Short)      \
    X(Float)      \
    X(Identifier) \
    X(Void)

#define X(name) name,

enum class Type : uint8_t {
    TYPE_ENUM
};

#undef X

constexpr const char* TypeEnumToString(Type e) noexcept
{
#define X(name)        \
    case (Type::name): \
        return #name;

    switch (e) {
        TYPE_ENUM
    }
#undef X
}

// base type
class Object {

public:
    Object(Type t)
        : type(t) {};
    virtual ~Object() {};

    virtual Object*     evalBinaryExpr(TokenType& type, Object* rval) = 0;
    virtual std::string toString()                                    = 0;

    Type type;
};

struct Value {
    Type type;
    union {
        double   f;
        uint32_t l;
    } value;
};

#define AS_FLOAT(val) \
    (((val).type == Type::Float) ? (val).value.f : (double)(val).value.l)

#define AS_INT(val) \
    (((val).type == Type::Integer) ? (val).value.l : (uint32_t)(val).value.f)

#define AS_BOOL(val) \
    ((val).type == Type::Integer ? (val).value.l != 0 : (val).value.f != 0)

Value operator+(Value& l, Value& r);
}

#pragma once

#include "base.hpp"

namespace Script {
class Reference : public Object {
public:
    Reference(uint32_t val)
        : Object(Type::Reference)
        , value(val) {};
    uint32_t value;

    Object*     evalBinaryExpr(TokenType& type, Object* operand);
    std::string toString()
    {
        return std::to_string(value);
    }
};
}

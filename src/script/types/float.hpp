#pragma once

#include "base.hpp"

namespace Script {
class Float : public Object {
public:
    Float(float val)
        : Object(Type::Float)
        , value(val) {};
    float value;

    Object*     evalBinaryExpr(TokenType& type, Object* operand);
    std::string toString()
    {
        return std::to_string(value);
    }
};
}

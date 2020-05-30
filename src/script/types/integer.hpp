#pragma once

#include "base.hpp"

namespace Script {
class Integer : public Object {
public:
    Integer(int32_t val)
        : Object(Type::Integer)
        , value(val) {};
    int32_t value;

    Object*     evalBinaryExpr(TokenType& type, Object* rval);
    std::string toString()
    {
        return std::to_string(value);
    }
};
}

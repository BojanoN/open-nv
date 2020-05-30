#pragma once

#include "base.hpp"
#include <string>

namespace Script {
class Identifier : public Object {
public:
    Identifier(std::string& val)
        : Object(Type::Identifier)
        , value(val) {};
    std::string value;
    Object*     evalBinaryExpr(TokenType& type, Object* operand);
    std::string toString()
    {
        return value;
    }
};
}

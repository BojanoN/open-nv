#include "identifier.hpp"

namespace Script {
Object* Identifier::evalBinaryExpr(TokenType& type, Object* operand)
{
    throw std::runtime_error("Identifier arithmetic is not allowed");

    return nullptr;
}
}

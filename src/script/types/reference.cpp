#include "reference.hpp"

namespace Script {
Object* Reference::evalBinaryExpr(TokenType& type, Object* operand)
{
    throw std::runtime_error("Reference arithmetic is not allowed");

    return nullptr;
}

}

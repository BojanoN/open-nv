#include "float.hpp"
#include "integer.hpp"

namespace Script {
Object* Float::evalBinaryExpr(TokenType& operatorType, Object* operand)
{
    if (operand->type != Type::Integer && operand->type != Type::Float) {
        log_fatal("Cannot coerce type %s to float", TypeEnumToString(operand->type));
        throw std::runtime_error("Type mismatch!");
    }

    float result = 0;
    float lval   = this->value;
    float rval   = 0;

    switch (operand->type) {
    case Type::Integer:
        rval = (float)dynamic_cast<Integer*>(operand)->value;
        break;
    case Type::Float:
        rval = dynamic_cast<Float*>(operand)->value;
        break;
    default:
        break;
    }

    switch (operatorType) {
    case (TokenType::Plus):
        result = lval + rval;
        break;
    case (TokenType::Minus):
        result = lval - rval;
        break;
    case (TokenType::Asterisk):
        result = lval * rval;
        break;
    case (TokenType::Division):
        result = lval / rval;
        break;
    case (TokenType::GreaterThan):
        result = lval > rval;
        break;
    case (TokenType::GreaterThanOrEqualTo):
        result = lval >= rval;
        break;
    case (TokenType::LessThan):
        result = lval < rval;
        break;
    case (TokenType::LessThanOrEqualTo):
        result = lval <= rval;
        break;
    case (TokenType::EqualTo):
        result = lval == rval;
        break;
    case (TokenType::NotEqualTo):
        result = lval != rval;
        break;
    default:
        log_fatal("Operator %s is not supported for type float", TokenEnumToString(operatorType));
        throw std::runtime_error("Unsupported operator");
    }

    return new Float(result);
}
}

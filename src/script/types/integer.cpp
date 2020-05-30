#include "integer.hpp"
#include "float.hpp"

namespace Script {

Object* Integer::evalBinaryExpr(TokenType& operatorType, Object* operand)
{
    if (operand->type != Type::Integer && operand->type != Type::Float) {
        log_fatal("Cannot coerce type %s to integer", TypeEnumToString(operand->type));
        throw std::runtime_error("Type mismatch!");
    }

    int32_t result = 0;
    int32_t lval   = this->value;
    int32_t rval   = 0;

    switch (operand->type) {
    case Type::Integer:
        rval = dynamic_cast<Integer*>(operand)->value;
        break;
    case Type::Float:
        rval = (int32_t) dynamic_cast<Float*>(operand)->value;
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
    case (TokenType::Or):
        result = lval || rval;
        break;
    case (TokenType::And):
        result = lval && rval;
        break;
    default:
        log_fatal("Operator %s is not supported for type integer", TokenEnumToString(operatorType));
        throw std::runtime_error("Unsupported operator");
    }

    return new Integer(result);
}

};

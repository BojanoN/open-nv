#include "expr.hpp"
#include <iostream>

namespace Script {
void BinaryExpr::print()
{
    std::cout << "(";
    this->left->print();
    std::cout << " " << TokenEnumToString(this->op.type) << " ";
    this->right->print();
    std::cout << ")";
}

void GroupingExpr::print()
{
    this->expression->print();
}

ExprValue GroupingExpr::eval(Context& context)
{
    return this->expression->eval(context);
}

ExprValue BinaryExpr::eval(Context& context)
{
    ExprValue ret;

    ExprValue lval = this->left->eval(context);
    ExprValue rval = this->right->eval(context);

    if (lval.type == TokenType::Identifier || rval.type == TokenType::Identifier) {
        throw std::runtime_error("Operands must be numbers!");
    }

    float fRes = 0;

    switch (this->op.type) {
    case (TokenType::Plus):
        fRes = lval.f + rval.f;
        break;
    case (TokenType::Minus):
        fRes = lval.f - rval.f;
        break;
    case (TokenType::Asterisk):
        fRes = lval.f * rval.f;
        break;
    case (TokenType::Division):
        fRes = lval.f / rval.f;
        break;
    case (TokenType::GreaterThan):
        fRes = lval.f > rval.f;
        break;
    case (TokenType::GreaterThanOrEqualTo):
        fRes = lval.f >= rval.f;
        break;
    case (TokenType::LessThan):
        fRes = lval.f < rval.f;
        break;
    case (TokenType::LessThanOrEqualTo):
        fRes = lval.f <= rval.f;
        break;
    case (TokenType::EqualTo):
        fRes = (int)lval.f == (int)rval.f;
        break;
    case (TokenType::NotEqualTo):
        fRes = (int)lval.f != (int)rval.f;
        break;
    case (TokenType::Or):
        fRes = (int)lval.f | (int)rval.f;
        break;
    case (TokenType::And):
        fRes = (int)lval.f & (int)rval.f;
        break;
    default:
        throw std::runtime_error("Whoops");
    }

    ret.f    = fRes;
    ret.type = TokenType::Float;
    ret.s    = std::to_string(fRes);

    return ret;
}

ExprValue LiteralExpr::eval(Context& context)
{
    ExprValue ret(this->type);
    ret.s = this->value;

    switch (this->type) {
    case (TokenType::Integer):
        // Forcing everything to float for now
        //  ret.i = std::stoi(this->value);
        ret.f = std::stoi(this->value);
        break;
    case (TokenType::Float):
        ret.f = std::stof(this->value);
        break;
    case (TokenType::Identifier):
        break;
    default:
        throw std::runtime_error("Whoops");
    }

    return ret;
}

}

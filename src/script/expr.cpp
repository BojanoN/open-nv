#include "expr.hpp"
#include "context.hpp"

#include "types/float.hpp"
#include "types/identifier.hpp"
#include "types/integer.hpp"

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

Object* GroupingExpr::eval(Context& context)
{
    return this->expression->eval(context);
}

Object* BinaryExpr::eval(Context& context)
{
    Object* ret;

    Object* lval = this->left->eval(context);
    Object* rval = this->right->eval(context);

    if (lval->type == Type::Identifier || rval->type == Type::Identifier) {
        throw std::runtime_error("This should never happen");
    }

    ret = lval->evalBinaryExpr(this->op.type, rval);

    delete lval;
    delete rval;

    return ret;
}

Object* LiteralExpr::eval(Context& context)
{
    Object* ret;

    switch (this->type) {
    case (TokenType::Integer):
        ret = new Integer(std::stoi(this->value));
        break;
    case (TokenType::Float):
        ret = new Float(std::stof(this->value));
        break;
    case (TokenType::Identifier):
        ret = new Identifier(this->value);
        break;
    default:
        throw std::runtime_error("Whoops");
    }

    return ret;
}

Object* Assignment::eval(Context& context)
{

    // TODO: variable assignments

    if (!context.varExists(this->variable)) {
        throw std::runtime_error("Undefined variable");
    }

    return this->expression->eval(context);
}

}

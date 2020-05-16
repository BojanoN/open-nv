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

void LiteralExpr::print()
{
    std::cout << this->value;
}
}

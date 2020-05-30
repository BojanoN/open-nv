#include "statement.hpp"
#include "context.hpp"
#include "expr.hpp"

namespace Script {

ExpressionStatement::~ExpressionStatement()
{
    delete this->expression;
}

void ExpressionStatement::execute(Context& context)
{
}

void Variable::execute(Context& context)
{
}
}

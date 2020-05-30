#include "statement.hpp"
#include "context.hpp"
#include "expr.hpp"

namespace Script {

ExpressionStatement::~ExpressionStatement()
{
    delete this->expression;
}

IfStatement::~IfStatement()
{
    delete condition;
    delete body;
    if (elseBody != nullptr) {
        delete elseBody;
    }
};

void ExpressionStatement::execute(Context& context)
{
}

void Variable::execute(Context& context)
{
}
}

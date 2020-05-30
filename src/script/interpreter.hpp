#pragma once

#include "context.hpp"
#include "expr.hpp"
#include "logc/log.h"
#include "statement.hpp"
#include "types/base.hpp"

namespace Script {

class Interpreter {

public:
    Interpreter() {};
    ~Interpreter() {};

    void interpret(Expr* expression)
    {
        try {
            Script::Object* tmp = expression->eval(this->context);
            log_info("Expression value: %s", tmp->toString().c_str());
            delete tmp;
        } catch (std::runtime_error& e) {
            log_fatal("Expression evaluation error: %s", e.what());
        }
    };

private:
    Context context;
};
}

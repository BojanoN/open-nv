#pragma once

#include "expr.hpp"
#include "logc/log.h"

namespace Script {

class Interpreter {

public:
    Interpreter() {};
    ~Interpreter() {};

    void interpret(Expr* expression)
    {
        try {
            Script::ExprValue tmp = expression->eval();
            log_info("Expression value: %s", tmp.s.c_str());
        } catch (std::runtime_error& e) {
            log_fatal("Expression evaluation error: %s", e.what());
        }
    };
};
}

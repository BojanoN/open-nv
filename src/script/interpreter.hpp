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

    void interpret(Node* expression) {};

private:
    Context context;
};
}

#pragma once

#include "context.hpp"
#include "logc/log.h"
#include "node.hpp"
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

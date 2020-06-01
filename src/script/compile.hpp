#pragma once

#include "context.hpp"

namespace Script {
class Compiler {
public:
    Compiler() {};

    void compile();

private:
    Context ctx;

    void compileFuncCall();
    void compileExpr();
    void compileDecl();
    void compileIf();
};
}

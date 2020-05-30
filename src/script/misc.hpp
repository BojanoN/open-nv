#pragma once

#include "tokenizer.hpp"
#include <string>

namespace Script {
struct ExprValue {

public:
    ExprValue(TokenType t)
        : i(0)
        , f(0.)
        , s("")
        , type(t)
    {
    }

    ExprValue()
        : i(0)
        , f(0.)
        , s("")
        , type(TokenType::Identifier)
    {
    }

    int32_t     i;
    float       f;
    bool        b;
    std::string s;
    TokenType   type;

    void print()
    {
        std::cout << s;
    }
};
}

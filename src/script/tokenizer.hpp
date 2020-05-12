#pragma once
#include <cstdint>
#include <string>

namespace Script {

enum class TokenType : uint8_t {
    Number,
    Long,
    Short,
    Int,
    Reference,

    Var,
    Function,
    GlobalVar,

    Operator,
    Boolean,
    String,

};

class Token {

private:
    TokenType type;
};

class Tokenizer {
public:
    std::vector<Token>* getTokens(std::string& source);

private:
    void error(std::string& message, unsigned int line);

    unsigned int current;
    bool         tokenizeError;
};
};

#pragma once
#include "expr.hpp"
#include "logc/log.h"
#include "tokenizer.hpp"
#include <set>

namespace Script {
class Parser {
public:
    Parser(Tokenizer* tokenizer)
        : tokens(tokenizer->tokens)
        , currentOffset(0)
    {
    }

    Expr* parse()
    {
        return this->expression();
    }

private:
    uint32_t            currentOffset;
    std::vector<Token>& tokens;

    void error(std::string cause, Token& tok)
    {
        std::string errorMsg = "Syntax error: ";
        errorMsg += cause;
        errorMsg += " at line " + std::to_string(tok.line);
        errorMsg += " at column " + std::to_string(tok.column);
        throw std::runtime_error(errorMsg);
    }

    bool end()
    {
        return this->tokens[currentOffset].type == TokenType::Eof;
    }

    Token& advance()
    {
        this->currentOffset++;
        return this->tokens[currentOffset];
    }

    bool advanceMatches(std::set<TokenType>& tokens)
    {
        if (tokens.count(this->peekCurrent().type) && !this->end()) {
            advance();
            return true;
        }

        return false;
    }

    bool advanceMatches(TokenType type)
    {
        if (type == this->peekCurrent().type && !this->end()) {
            advance();
            return true;
        }

        return false;
    }

    Token& peekCurrent()
    {
        return tokens[currentOffset];
    }

    Token& peekNext()
    {
        return tokens[currentOffset + 1];
    }

    Token& previous()
    {
        return tokens[currentOffset - 1];
    }

    void check(TokenType type, std::string errorMsg)
    {
        Token& tok = this->peekCurrent();
        log_debug("check type: %s, current type: %s", TokenEnumToString(type), TokenEnumToString(tok.type));

        if (tok.type == type) {
            advance();
            return;
        }

        error(errorMsg, tok);
    }

    Expr* expression();
    Expr* equals();
    Expr* comparison();
    Expr* addition();
    Expr* multiplication();
    Expr* baseType();
};
};

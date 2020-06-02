#pragma once
#include "logc/log.h"
#include "node.hpp"
#include "tokenizer.hpp"
#include <set>
#include <unordered_map>

namespace Script {
class Parser {
public:
    Parser(Tokenizer* tokenizer)
        : tokens(tokenizer->tokens)
        , currentOffset(0)
    {
    }
    std::vector<Node*>* parse();

private:
    uint32_t                     currentOffset;
    std::vector<Token>&          tokens;
    static std::set<std::string> blocktypes;

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

    void check(std::set<TokenType>& types, std::string errorMsg)
    {
        Token& tok = this->peekCurrent();

        if (types.count(tok.type)) {
            advance();
            return;
        }

        error(errorMsg, tok);
    }

    void checkNext(TokenType type, std::string errorMsg)
    {
        Token& tok = this->peekNext();
        log_debug("check type: %s, next type: %s", TokenEnumToString(type), TokenEnumToString(tok.type));

        if (tok.type == type) {
            advance();
            return;
        }

        error(errorMsg, tok);
    }

    Node* statement();
    Node* expressionStatement();
    Node* declaration();
    Node* varDeclaration();
    Node* ifStatement();

    Node* functionCall();
    Node* assignment();
    Node* expression();
    Node* equals();
    Node* comparison();
    Node* addition();
    Node* multiplication();
    Node* baseType();
};
};

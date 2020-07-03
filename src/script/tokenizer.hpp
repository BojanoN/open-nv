#pragma once

#include "logc/log.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Script {

#define TOKEN_ENUM          \
    X(Number)               \
    X(Long)                 \
    X(Short)                \
    X(Integer)              \
    X(Reference)            \
    X(Float)                \
    X(String)               \
                            \
    X(FloatConstant)        \
    X(IntegerConstant)      \
    X(Var)                  \
                            \
    X(Operator)             \
    X(BlockType)            \
                            \
    X(ScriptName)           \
    X(If)                   \
    X(Else)                 \
    X(Endif)                \
    X(Elseif)               \
    X(Begin)                \
    X(End)                  \
    X(Set)                  \
    X(To)                   \
                            \
    X(Plus)                 \
    X(Minus)                \
    X(Asterisk)             \
    X(Division)             \
    X(Modulo)               \
    X(EqualTo)              \
    X(NotEqualTo)           \
    X(GreaterThan)          \
    X(LessThan)             \
    X(GreaterThanOrEqualTo) \
    X(LessThanOrEqualTo)    \
    X(And)                  \
    X(Or)                   \
    X(Dot)                  \
                            \
    X(LeftParenthesis)      \
    X(RightParenthesis)     \
                            \
    X(Newline)              \
    X(Identifier)           \
    X(Return)               \
    X(Eof)                  \
    X(Comma)

#define X(name) name,

enum class TokenType : uint8_t {
    TOKEN_ENUM
};

#undef X

constexpr const char* TokenEnumToString(TokenType e) noexcept
{
#define X(name)             \
    case (TokenType::name): \
        return #name;

    switch (e) {
        TOKEN_ENUM
    }
#undef X
}

class Token {
public:
    Token(uint32_t lineNum, std::string lit, TokenType tType, uint32_t col)
        : line(lineNum)
        , literal(lit)
        , type(tType)
        , column(col) {};

    Token(uint32_t lineNum, TokenType tType, uint32_t col)
        : line(lineNum)
        , literal("")
        , type(tType)
        , column(col)
    {
    }

    friend std::ostream& operator<<(std::ostream& out, const Token& t);
    friend class Parser;
    friend class Compiler;

    bool operator==(const Token& other) const
    {
        return this->type == other.type;
    }

    bool operator<(const Token& other) const
    {
        return this->type < other.type;
    }

    TokenType type;

private:
    uint32_t    line;
    uint32_t    column;
    std::string literal;
};

class Tokenizer {
public:
    std::vector<Token>* getTokens(std::string& src);

    Tokenizer()
        : currentLine(0)
        , currentColumn(0)
        , currentCharOffset(0)
        , startCharOffset(0)
        , sourceSize(0)
        , tokens(nullptr)
        , tokenizeError(false)
    {
        log_debug("%s", source.c_str());
    };

    friend class Parser;

    bool hadError()
    {
        return this->tokenizeError;
    }

private:
    void error(std::string message);

    bool end()
    {
        return currentCharOffset > (sourceSize - 1);
    }

    char advance()
    {

        this->currentCharOffset++;
        this->currentColumn++;
        return this->source[currentCharOffset];
    }

    char peekNext()
    {
        return this->source[currentCharOffset + 1];
    }

    char previous()
    {
        if (this->currentCharOffset - 1 > 0) {
            return this->source[this->currentCharOffset - 1];
        }
        return 0;
    }

    char peekCurrent()
    {
        return this->source[currentCharOffset];
    }

    bool advanceMatches(char c)
    {
        if (this->end()) {
            return false;
        }

        if (this->source[currentCharOffset + 1] == c) {
            this->currentCharOffset++;
            return true;
        }

        return false;
    }

    void parseStringLiteral();
    void parseLiteral();
    void parseNumber();

    bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    bool isAlpha(char c)
    {
        return (c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z')
            || c == '_';
    }

    bool isAlphaNum(char c)
    {
        return isAlpha(c) || isDigit(c);
    }

    void toLowerCase(std::string& str)
    {
        uint32_t size = str.size();

        for (uint32_t i = 0; i < size; i++) {
            str[i] = std::tolower(str[i]);
        }
    }

    void reset()
    {
        currentLine       = 1;
        currentColumn     = 0;
        currentCharOffset = 0;
        startCharOffset   = 0;
        tokenizeError     = false;
        sourceSize        = source.size();
    }

    std::string substring(std::string& str, unsigned int beg, unsigned int end)
    {
        std::string ret;
        ret.resize(end - beg);

        std::strncpy(const_cast<char*>(ret.data()), const_cast<char*>(str.data()) + beg, end - beg);

        return ret;
    }

    unsigned int currentLine;
    unsigned int currentColumn;
    uint32_t     sourceSize;
    uint32_t     currentCharOffset;
    uint32_t     startCharOffset;

    std::vector<Token>*                               tokens;
    static std::unordered_map<std::string, TokenType> keywords;

    std::string source;
    bool        tokenizeError;
};
};

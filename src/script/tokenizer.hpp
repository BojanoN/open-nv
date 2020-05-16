#pragma once

#include <cctype>
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
    X(Int)                  \
    X(Reference)            \
    X(Float)                \
    X(String)               \
                            \
    X(Var)                  \
    X(Function)             \
    X(GlobalVar)            \
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
    X(Identifier)           \
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
    Token(uint32_t lineNum, std::string lit, std::string lex, TokenType tType)
        : line(lineNum)
        , lexeme(lex)
        , literal(lit)
        , type(tType) {};

    Token(uint32_t lineNum, TokenType tType)
        : line(lineNum)
        , literal("")
        , lexeme("")
        , type(tType)
    {
    }

    friend std::ostream& operator<<(std::ostream& out, const Token& t);
    friend class Parser;

    bool operator==(const Token& other) const
    {
        return this->type == other.type;
    }

    bool operator<(const Token& other) const
    {
        return this->type < other.type;
    }

private:
    uint32_t    line;
    std::string literal;
    std::string lexeme;
    TokenType   type;
};

class Tokenizer {
public:
    void getTokens();

    void printTokens()
    {
        for (Token t : this->tokens) {
            std::cout << t << "\n";
        }
    }

    Tokenizer(std::string& script)
        : source(script)
        , currentLine(0)
        , currentColumn(0)
        , currentCharOffset(0)
        , startCharOffset(0)
        , sourceSize(script.size())
        , tokenizeError(false) {};

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

    std::string substring(std::string& str, unsigned int beg, unsigned int end)
    {
        std::string ret;
        ret.resize(end - beg);

        std::strncpy(ret.data(), str.data() + beg, end - beg);

        return ret;
    }

    unsigned int currentLine;
    unsigned int currentColumn;
    uint32_t     sourceSize;
    uint32_t     currentCharOffset;
    uint32_t     startCharOffset;

    std::vector<Token>                                tokens;
    static std::unordered_map<std::string, TokenType> keywords;

    std::string source;
    bool        tokenizeError;
};
};

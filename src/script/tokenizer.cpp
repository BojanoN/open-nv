#include "tokenizer.hpp"
#include "logc/log.h"
#include <set>

#define ADD_EMPTY_TOKEN(type) this->tokens->emplace_back(Token((this->currentLine), (type), (this->currentColumn)))

#define ADD_TOKEN(type, value) this->tokens->emplace_back(Token((this->currentLine), (value), (type), (this->currentColumn)))

#define UNK_CHAR_ERROR()                                 \
    errorMessage = "Unknown character in offset ";       \
    errorMessage += std::to_string(this->currentColumn); \
    this->error(errorMessage)

static std::set<Script::TokenType> varTypeMatch = {
    Script::TokenType::Reference,
    Script::TokenType::Integer,
    Script::TokenType::Short,
    Script::TokenType::Float,
    Script::TokenType::Long
};

namespace Script {

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << TokenEnumToString(t.type) << ", " << t.literal;

    return os;
}

void Tokenizer::error(std::string message)
{
    delete this->tokens;
    this->tokens = nullptr;

    log_fatal("Tokenizer: %s in line %u", message.c_str(), this->currentLine);
    this->tokenizeError = true;
}

void Tokenizer::parseStringLiteral()
{
    this->startCharOffset = currentCharOffset;

    while (this->peekNext() != '"' && !this->end()) {
        if (this->peekCurrent() == '\n') {
            this->error("Unterminated string");
            return;
        }
        advance();
    }

    ADD_TOKEN(TokenType::String, this->substring(this->source, this->startCharOffset + 1, currentCharOffset + 1));
    advance();
}

void Tokenizer::parseNumber()
{
    bool isFloat = false;

    this->startCharOffset = this->currentCharOffset;

    while (this->isDigit(this->peekNext())) {
        this->advance();
    }

    if (this->peekNext() == '.') {
        isFloat = true;
        this->advance();

        while (this->isDigit(this->peekNext())) {
            this->advance();
        }
    }

    ADD_TOKEN(isFloat ? TokenType::FloatConstant : TokenType::IntegerConstant, this->substring(this->source, this->startCharOffset, currentCharOffset + 1));
};

void Tokenizer::parseLiteral()
{
    this->startCharOffset = this->currentCharOffset;

    while (this->isAlphaNum(this->peekNext())) {
        advance();
    }
    std::string value = this->substring(this->source, this->startCharOffset, this->currentCharOffset + 1);
    this->toLowerCase(value);

    if (Tokenizer::keywords.count(value)) {
        TokenType tmp = Tokenizer::keywords[value];
        ADD_TOKEN(tmp, value);
    } else {
        ADD_TOKEN(TokenType::Identifier, value);
    }
}

std::vector<Token>* Tokenizer::getTokens(std::string& src)
{
    this->tokens = new std::vector<Token>();
    this->source = src;
    reset();

    char        current = peekCurrent();
    std::string errorMessage;

    this->currentLine = 1;

    while (!this->end()) {
        switch (current) {
        case ('+'):
            ADD_TOKEN(TokenType::Plus, "+");
            break;
        case ('-'):
            ADD_TOKEN(TokenType::Minus, "-");
            break;
        case ('*'):
            ADD_TOKEN(TokenType::Asterisk, "*");
            break;
        case ('.'):
            ADD_TOKEN(TokenType::Dot, "/");
            break;
        case ('('):
            ADD_EMPTY_TOKEN(TokenType::LeftParenthesis);
            break;
        case (')'):
            ADD_EMPTY_TOKEN(TokenType::RightParenthesis);
            break;
        case ('!'):
            if (advanceMatches('=')) {
                ADD_TOKEN(TokenType::NotEqualTo, "!=");
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('='):
            if (advanceMatches('=')) {
                ADD_TOKEN(TokenType::EqualTo, "==");
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('&'):
            if (advanceMatches('&')) {
                ADD_TOKEN(TokenType::And, "&&");
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('|'):
            if (advanceMatches('|')) {
                ADD_TOKEN(TokenType::Or, "||");
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('<'): {
            bool isLte = advanceMatches('=');
            ADD_TOKEN(isLte ? TokenType::LessThanOrEqualTo : TokenType::LessThan, isLte ? "<=" : "<");
            break;
        }
        case ('>'): {
            bool isGte = advanceMatches('=');
            ADD_TOKEN(isGte ? TokenType::GreaterThanOrEqualTo : TokenType::GreaterThan, isGte ? ">=" : ">");
            break;
        }
        case (';'):
            while (this->peekCurrent() != '\n' && !this->end()) {
                advance();
            }
            break;
        case ('\n'):
            this->currentLine++;
            this->currentColumn = 0;
            ADD_EMPTY_TOKEN(TokenType::Newline);
            break;
        case (' '):
        case ('\r'):
        case ('\t'):
            break;
        //        case ('"'):
        //   this->parseStringLiteral();
        //    break;
        case (','):
            ADD_EMPTY_TOKEN(TokenType::Comma);
            break;
        default:
            if (this->isDigit(current)) {
                this->parseNumber();
            } else if (this->isAlpha(current)) {
                this->parseLiteral();
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        }
        current = advance();
    }
    ADD_EMPTY_TOKEN(TokenType::Eof);

    std::vector<Token>* ret = this->tokens;
    this->tokens            = nullptr;

    return ret;
}

std::unordered_map<std::string, TokenType> Tokenizer::keywords = {
    { "scriptname", TokenType::ScriptName },
    { "scn", TokenType::ScriptName },
    { "if", TokenType::If },
    { "else", TokenType::Else },
    { "elseif", TokenType::Elseif },
    { "endif", TokenType::Endif },
    { "begin", TokenType::Begin },
    { "end", TokenType::End },
    { "set", TokenType::Set },
    { "to", TokenType::To },
    { "ref", TokenType::Reference },
    { "int", TokenType::Integer },
    { "long", TokenType::Long },
    { "short", TokenType::Short },
    { "float", TokenType::Float },

};
};

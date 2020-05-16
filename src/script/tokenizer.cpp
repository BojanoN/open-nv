#include "tokenizer.hpp"
#include "logc/log.h"

#define ADD_EMPTY_TOKEN(type) this->tokens.emplace_back(Token((this->currentLine), (type)))

#define ADD_TOKEN(type, value) this->tokens.emplace_back(Token((this->currentLine), (value), "", (type)))

#define UNK_CHAR_ERROR()                                 \
    errorMessage = "Unknown character in offset ";       \
    errorMessage += std::to_string(this->currentColumn); \
    this->error(errorMessage)

namespace Script {

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << "Line: " << t.line << "\n"
       << "Value: " << t.literal << "\n"
       //       << "Lexeme: " << t.lexeme << "\n"
       << "Type: " << TokenEnumToString(t.type) << "\n";

    return os;
}

void Tokenizer::error(std::string message)
{
    log_fatal("Tokenizer: %s in line %u", message.c_str(), this->currentLine);
    this->tokenizeError = true;
}

void Tokenizer::parseStringLiteral()
{
    char c;
    this->startCharOffset = currentCharOffset;

    while ((c = this->peekCurrent()) != '"' && !this->end()) {
        if (c == '\n') {
            this->error("Unterminated string");
            return;
        }
        advance();
    }

    ADD_TOKEN(TokenType::String, this->substring(this->source, this->startCharOffset + 1, currentCharOffset));
    advance();
}

void Tokenizer::parseNumber()
{
    bool isFloat = false;

    this->startCharOffset = this->currentCharOffset;

    while (this->isDigit(this->peekCurrent())) {
        this->advance();
    }

    if (this->peekCurrent() == '.' && this->isDigit(this->peekNext())) {
        isFloat = true;
        this->advance();

        while (this->isDigit(this->peekCurrent())) {
            this->advance();
        }
    }

    ADD_TOKEN(TokenType::Number, this->substring(this->source, this->startCharOffset, currentCharOffset));
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
        ADD_TOKEN(Tokenizer::keywords[value], value);
    } else {
        ADD_TOKEN(TokenType::Identifier, value);
    }
}

void Tokenizer::getTokens()
{
    char        current = peekCurrent();
    std::string errorMessage;

    this->currentLine = 1;

    while (!this->end()) {
        switch (current) {
        case ('+'):
            ADD_EMPTY_TOKEN(TokenType::Plus);
            break;
        case ('-'):
            ADD_EMPTY_TOKEN(TokenType::Minus);
            break;
        case ('*'):
            ADD_EMPTY_TOKEN(TokenType::Asterisk);
            break;
        case ('.'):
            ADD_EMPTY_TOKEN(TokenType::Dot);
            break;
        case ('('):
            ADD_EMPTY_TOKEN(TokenType::LeftParenthesis);
            break;
        case (')'):
            ADD_EMPTY_TOKEN(TokenType::RightParenthesis);
            break;
        case ('!'):
            if (advanceMatches('=')) {
                ADD_EMPTY_TOKEN(TokenType::NotEqualTo);
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('='):
            if (advanceMatches('=')) {
                ADD_EMPTY_TOKEN(TokenType::EqualTo);
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('&'):
            if (advanceMatches('&')) {
                ADD_EMPTY_TOKEN(TokenType::And);
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('|'):
            if (advanceMatches('|')) {
                ADD_EMPTY_TOKEN(TokenType::Or);
            } else {
                UNK_CHAR_ERROR();
            }
            break;
        case ('<'):
            ADD_EMPTY_TOKEN(advanceMatches('=') ? TokenType::LessThanOrEqualTo : TokenType::LessThan);
            break;
        case ('>'):
            ADD_EMPTY_TOKEN(advanceMatches('=') ? TokenType::GreaterThanOrEqualTo : TokenType::GreaterThan);
            break;
        case (';'):
            while (this->peekCurrent() != '\n' && !this->end()) {
                advance();
            }
            break;
        case ('\n'):
            this->currentLine++;
            this->currentColumn = 0;
            break;
        case (' '):
        case ('\r'):
        case ('\t'):
            break;
        case ('"'):
            this->parseStringLiteral();
            break;
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
}

std::unordered_map<std::string, TokenType> Tokenizer::keywords = {
    { "scriptname", TokenType::ScriptName },
    { "scn", TokenType::ScriptName },
    { "if", TokenType::If },
    { "else", TokenType::Else },
    { "elseif", TokenType::Elseif },
    { "endif", TokenType::Else },
    { "begin", TokenType::Begin },
    { "end", TokenType::End },
    { "set", TokenType::Set },
    { "to", TokenType::To },
};

};

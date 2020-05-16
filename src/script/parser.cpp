#include "parser.hpp"

static std::set<Script::TokenType> equalsMatch     = { Script::TokenType::EqualTo, Script::TokenType::NotEqualTo };
static std::set<Script::TokenType> comparisonMatch = {
    Script::TokenType::GreaterThan,
    Script::TokenType::GreaterThanOrEqualTo,
    Script::TokenType::LessThan,
    Script::TokenType::LessThanOrEqualTo
};
static std::set<Script::TokenType> additionMatch       = { Script::TokenType::Plus, Script::TokenType::Minus };
static std::set<Script::TokenType> multiplicationMatch = { Script::TokenType::Asterisk, Script::TokenType::Division };
static std::set<Script::TokenType> baseTypeMatch       = { Script::TokenType::String, Script::TokenType::Number };

namespace Script {
Expr* Parser::expression()
{
    return this->equals();
}

Expr* Parser::equals()
{
    Expr* ret = this->comparison();

    while (this->advanceMatches(equalsMatch)) {
        Token& op            = this->previous();
        Expr*  rightHandExpr = this->comparison();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Expr* Parser::comparison()
{
    Expr* ret = this->addition();

    while (this->advanceMatches(comparisonMatch)) {
        Token& op            = this->previous();
        Expr*  rightHandExpr = this->addition();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Expr* Parser::addition()
{
    Expr* ret = this->multiplication();

    while (this->advanceMatches(comparisonMatch)) {
        Token& op            = this->previous();
        Expr*  rightHandExpr = this->multiplication();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Expr* Parser::multiplication()
{
    Expr* ret = this->baseType();

    while (this->advanceMatches(multiplicationMatch)) {
        Token& op            = this->previous();
        Expr*  rightHandExpr = this->baseType();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Expr* Parser::baseType()
{
    if (advanceMatches(baseTypeMatch)) {
        return new LiteralExpr(this->previous().literal);
    }

    if (advanceMatches(TokenType::LeftParenthesis)) {
        Expr* expr = expression();

        try {
            this->check(TokenType::RightParenthesis, "Expected )");
            return new GroupingExpr(expr);
        } catch (std::runtime_error& e) {
            delete expr;
            throw e;
        }
    }

    error("Expected expression", this->peekCurrent());
    // Silence end-of-control warning
    return nullptr;
}

};

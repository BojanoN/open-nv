#include "parser.hpp"
#include "logc/log.h"

static std::set<Script::TokenType> equalsMatch     = { Script::TokenType::EqualTo, Script::TokenType::NotEqualTo };
static std::set<Script::TokenType> comparisonMatch = {
    Script::TokenType::GreaterThan,
    Script::TokenType::GreaterThanOrEqualTo,
    Script::TokenType::LessThan,
    Script::TokenType::LessThanOrEqualTo
};
static std::set<Script::TokenType> additionMatch       = { Script::TokenType::Plus, Script::TokenType::Minus };
static std::set<Script::TokenType> multiplicationMatch = { Script::TokenType::Asterisk, Script::TokenType::Division };
static std::set<Script::TokenType> baseTypeMatch       = { Script::TokenType::Float, Script::TokenType::Identifier, Script::TokenType::Integer };

namespace Script {
Expr* Parser::expression()
{
#ifdef DEBUG
    log_debug("%s", "expression");
#endif
    return this->equals();
}

Expr* Parser::equals()
{
#ifdef DEBUG
    log_debug("%s", "equals");
#endif
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

#ifdef DEBUG
    log_debug("%s", "comparison");
#endif

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
#ifdef DEBUG
    log_debug("%s", "addition");
#endif

    Expr* ret = this->multiplication();

    while (this->advanceMatches(additionMatch)) {
        Token& op            = this->previous();
        Expr*  rightHandExpr = this->multiplication();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Expr* Parser::multiplication()
{
#ifdef DEBUG
    log_debug("%s", "multiplication");
#endif

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
#ifdef DEBUG
    log_debug("%s", "baseType");
#endif

    if (advanceMatches(baseTypeMatch)) {
        Token& tmp = previous();
        return new LiteralExpr(tmp.literal, tmp.type);
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

    error("Expected expression", this->peekNext());
    // Silence end-of-control warning
    return nullptr;
}

};

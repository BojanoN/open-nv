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
static std::set<Script::TokenType> baseTypeMatch       = { Script::TokenType::FloatConstant, Script::TokenType::Identifier, Script::TokenType::IntegerConstant };

static std::set<Script::TokenType> varTypeMatch = {
    Script::TokenType::Reference,
    Script::TokenType::Integer,
    Script::TokenType::Short,
    Script::TokenType::Float,
    Script::TokenType::Long
};

namespace Script {

std::set<std::string> Parser::blocktypes = {
    "function",
    "gamemode",
    "menumode",
    "onactivate",
    "onactorequip",
    "onactorunequip",
    "onadd",
    "onclose",
    "oncombatend",
    "ondeath",
    "ondestructionstagechange",
    "ondrop",
    "onequip",
    "onfire",
    "ongrab",
    "onhit",
    "onhitwith",
    "onload",
    "onmagiceffecthit",
    "onmurder",
    "onnpcactivate",
    "onopen",
    "onpackagechange",
    "onpackagedone",
    "onpackagestart",
    "onrelease",
    "onreset",
    "onsell",
    "onstartcombat",
    "ontrigger",
    "ontriggerenter",
    "ontriggerleave",
    "onunequip",
    "saytodone",
    "scripteffectfinish",
    "scripteffectstart",
    "scripteffectupdate"
};

Expr* Parser::expression()
{
#ifdef DEBUG
    log_debug("%s", "expression");
#endif
    return this->equals();
}

std::vector<Statement*>* Parser::parse()
{
    this->currentOffset = 0;
    std::vector<Statement*>* ret;
    Token&                   current = peekCurrent();
    std::string              blocktype;
    std::string              scriptName;

    if (current.type != TokenType::ScriptName) {
        this->error("Expected ScriptName keyword", current);
    }

    current = advance();

    if (current.type != TokenType::Identifier) {
        this->error("Expected script name", current);
    }

    scriptName = current.literal;
    checkNext(TokenType::Newline, "Expected newline");

    while (advanceMatches(TokenType::Newline)) { };
    current = peekCurrent();

    // Variable declaration block
    ret = new std::vector<Statement*>();

    while (varTypeMatch.count(current.type)) {
        Statement* tmp = varDeclaration();
        ret->push_back(tmp);
        while (advanceMatches(TokenType::Newline)) { };
        current = peekCurrent();
    };

    current = peekCurrent();

    if (current.type != TokenType::Begin) {
        delete ret;
        this->error("Expected Begin keyword", current);
    }

    current = advance();

    if (current.type != TokenType::Identifier) {
        delete ret;
        this->error("Expected blocktype identifier", current);
    }

    blocktype = current.literal;

    if (Parser::blocktypes.count(blocktype) == 0) {
        delete ret;
        this->error("Unknown blocktype", current);
    }

    bool hasEndBlock = false;
    current          = advance();

    // TODO: store blocktype parameters
    if (current.type == TokenType::Identifier) {
        log_info("Blocktype parameter: %s", current.literal.c_str());
        advance();
    }

    while (!end() && peekCurrent().type != TokenType::End) {

        while (advanceMatches(TokenType::Newline)) { }

        ret->emplace_back(statement());
    }

    if (end()) {
        delete ret;
        this->error("Missing End keyword", peekCurrent());
    }

    return ret;
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

Statement* Parser::statement()
{
    return expressionStatement();
}

Statement* Parser::expressionStatement()
{
    Expr* expr = assignment();

    checkNext(TokenType::Newline, "Expected newline after expression");
    advance();

    return new ExpressionStatement(expr);
}

Statement* Parser::varDeclaration()
{
    check(varTypeMatch, "Expected variable type");
    check(TokenType::Identifier, "Expected variable name");

    Token& varType = peekCurrent();
    Token& varName = advance();

    check(TokenType::Newline, "Expected newline after declaration");

    return new Variable(varType.type, varName.literal);
}

Expr* Parser::assignment()
{
    check(TokenType::Set, "Expected set keyword");
    Token&      varToken = peekCurrent();
    std::string var;

    var = varToken.literal;
    advance();
    check(TokenType::To, "Expected to keyword");
    Expr* expr = this->expression();

    return new Assignment(var, expr);
}

Statement* Parser::declaration()
{
    return statement();
}
};

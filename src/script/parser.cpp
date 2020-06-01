#include "parser.hpp"
#include "function.hpp"
#include "logc/log.h"

static std::set<Script::TokenType> equalsMatch     = { Script::TokenType::EqualTo, Script::TokenType::NotEqualTo };
static std::set<Script::TokenType> comparisonMatch = {
    Script::TokenType::GreaterThan,
    Script::TokenType::GreaterThanOrEqualTo,
    Script::TokenType::LessThan,
    Script::TokenType::LessThanOrEqualTo,
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

Node* Parser::expression()
{
#ifdef DEBUG
    log_debug("%s", "expression");
#endif
    return this->equals();
}

std::vector<Node*>* Parser::parse()
{
    this->currentOffset = 0;
    std::vector<Node*>* ret;
    Token&              current = peekCurrent();
    std::string         blocktype;
    std::string         scriptName;

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
    ret = new std::vector<Node*>();

    while (varTypeMatch.count(current.type)) {
        Node* tmp = varDeclaration();
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

    while (!end()) {

        while (advanceMatches(TokenType::Newline)) { }

        if (peekCurrent().type == TokenType::End) {
            break;
        }

        ret->emplace_back(statement());
    }

    if (end()) {
        delete ret;
        this->error("Missing End keyword", peekCurrent());
    }

    return ret;
}

Node* Parser::equals()
{
#ifdef DEBUG
    log_debug("%s", "equals");
#endif
    Node* ret = this->comparison();

    while (this->advanceMatches(equalsMatch)) {
        Token& op            = this->previous();
        Node*  rightHandExpr = this->comparison();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Node* Parser::comparison()
{

#ifdef DEBUG
    log_debug("%s", "comparison");
#endif

    Node* ret = this->addition();

    while (this->advanceMatches(comparisonMatch)) {
        Token& op            = this->previous();
        Node*  rightHandExpr = this->addition();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Node* Parser::addition()
{
#ifdef DEBUG
    log_debug("%s", "addition");
#endif

    Node* ret = this->multiplication();

    while (this->advanceMatches(additionMatch)) {
        Token& op            = this->previous();
        Node*  rightHandExpr = this->multiplication();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Node* Parser::multiplication()
{
#ifdef DEBUG
    log_debug("%s", "multiplication");
#endif

    Node* ret = this->functionCall();

    while (this->advanceMatches(multiplicationMatch)) {
        Token& op            = this->previous();
        Node*  rightHandExpr = this->baseType();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

Node* Parser::functionCall()
{
    Token& functionIdentifier = peekCurrent();
    log_info("DEBUG: %s", functionIdentifier.literal.c_str());
    if (FunctionResolver::functions.count(functionIdentifier.literal) == 0) {
        return baseType();
    }
    //FunctionInfo& info               = FunctionResolver::functions[functionIdentifier.value];
    advance();
    std::vector<Node*> arguments;

    while (peekCurrent().type != TokenType::Newline) {
        arguments.push_back(expression());
    }

    return new FunctionCallExpr(functionIdentifier.literal, arguments);
}

Node* Parser::baseType()
{
#ifdef DEBUG
    log_debug("%s", "baseType");
#endif

    if (advanceMatches(baseTypeMatch)) {
        Token& tmp = previous();
        return new LiteralExpr(tmp.literal, tmp.type);
    }

    if (advanceMatches(TokenType::LeftParenthesis)) {
        Node* expr = expression();

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

Node* Parser::statement()
{
    if (advanceMatches(TokenType::If))
        return ifStatement();

    return expressionStatement();
}

Node* Parser::expressionStatement()
{
    Node* expr;

    if (peekCurrent().type == TokenType::Set) {
        expr = assignment();
    } else {
        expr = expression();
    }

    check(TokenType::Newline, "Expected newline after expression statement");

    return new ExpressionStatement(expr);
}

Node* Parser::varDeclaration()
{
    Token& varType = peekCurrent();

    check(varTypeMatch, "Expected variable type");
    Token& varName = peekCurrent();
    check(TokenType::Identifier, "Expected variable name");

    advance();

    check(TokenType::Newline, "Expected newline after declaration");

    return new Variable(varType.type, varName.literal);
}

Node* Parser::assignment()
{
    check(TokenType::Set, "Expected set keyword");
    Token&      varToken = peekCurrent();
    std::string var;

    var = varToken.literal;
    advance();
    check(TokenType::To, "Expected to keyword");
    Node* expr = this->expression();

    return new Assignment(var, expr);
}

Node* Parser::declaration()
{
    return statement();
}

Node* Parser::ifStatement()
{
    Node* condition = expression();
    while (advanceMatches(TokenType::Newline)) { }
    log_debug("%s", TokenEnumToString(peekCurrent().type));

    Node* body = statement();

    log_debug("%s", TokenEnumToString(peekCurrent().type));

    Node* elseBody = nullptr;

    if (peekCurrent().type == TokenType::Else) {
        advance();
        while (advanceMatches(TokenType::Newline)) { };

        elseBody = statement();
    }

    check(TokenType::Endif, "Expected endif keyword");

    return new IfStatement(condition, body, elseBody);
}

};

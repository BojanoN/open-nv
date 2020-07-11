#include "parser.hpp"
#include "function.hpp"
#include "logc/log.h"
#include <utility>

static std::set<Script::TokenType> equalsMatch               = { Script::TokenType::EqualTo, Script::TokenType::NotEqualTo };
static std::set<Script::TokenType> comparisonAndLogicalMatch = {
    Script::TokenType::GreaterThan,
    Script::TokenType::GreaterThanOrEqualTo,
    Script::TokenType::LessThan,
    Script::TokenType::LessThanOrEqualTo,
    Script::TokenType::EqualTo,
    Script::TokenType::And,
    Script::TokenType::Or,
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

static std::set<Script::TokenType> functionCallArgType = {
    Script::TokenType::IntegerConstant,
    Script::TokenType::FloatConstant,
    Script::TokenType::Reference,
    Script::TokenType::Integer,
    Script::TokenType::Float,
    Script::TokenType::Identifier,
};

static std::unordered_map<Script::TokenType, Script::Type> tokenTypeToVarType = {
    { Script::TokenType::Reference, Script::Type::Reference },
    { Script::TokenType::IntegerConstant, Script::Type::Integer },
    { Script::TokenType::FloatConstant, Script::Type::Float },
    { Script::TokenType::Integer, Script::Type::Integer },
    { Script::TokenType::Short, Script::Type::Integer },
    { Script::TokenType::Float, Script::Type::Float },
    { Script::TokenType::Identifier, Script::Type::Identifier }
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

inline Node* Parser::expression()
{
#ifdef DEBUG
    log_debug("%s", "expression");
#endif
    return this->equals();
}

std::vector<Node*>* Parser::parse(std::vector<Token>* toks)
{
    this->currentOffset = 0;
    this->tokens        = toks;

    std::vector<Node*>* ret;

    Token& current = peekCurrent();
    ret            = new std::vector<Node*>();

    ret->push_back(scriptName());

    // Empty scripts are allowed
    if (end()) {
        return ret;
    }

    while (advanceMatches(TokenType::Newline)) { };
    current = peekCurrent();

    // Variable declaration block
    while (varTypeMatch.count(current.type)) {
        Node* tmp = varDeclaration();
        ret->push_back(tmp);
        while (advanceMatches(TokenType::Newline)) { };
        current = peekCurrent();
    };
    while (advanceMatches(TokenType::Newline)) { };

    // This allows scripts which only declare variables, without a script body
    // The use case is fairly common
    if (end()) {
        return ret;
    }

    ret->push_back(scriptBlock());

    if (end()) {
        for (Node* n : *ret) {
            delete n;
        }
        delete ret;

        this->error("Missing End keyword", peekCurrent());
    }

    return ret;
}

inline Node* Parser::equals()
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

inline Node* Parser::comparison()
{

#ifdef DEBUG
    log_debug("%s", "comparison");
#endif

    Node* ret = this->addition();

    while (this->advanceMatches(comparisonAndLogicalMatch)) {
        Token& op            = this->previous();
        Node*  rightHandExpr = this->addition();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

inline Node* Parser::addition()
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

inline Node* Parser::multiplication()
{
#ifdef DEBUG
    log_debug("%s", "multiplication");
#endif

    Node* ret = this->unary();

    while (this->advanceMatches(multiplicationMatch)) {
        Token& op            = this->previous();
        Node*  rightHandExpr = this->unary();
        ret                  = new BinaryExpr(op, ret, rightHandExpr);
    }

    return ret;
}

inline Node* Parser::unary()
{
#ifdef DEBUG
    log_debug("%s", "unary");
#endif

    if (this->advanceMatches(TokenType::Minus)) {
        Token& op            = this->previous();
        Node*  rightHandExpr = this->unary();
        return new UnaryExpr(op, rightHandExpr);
    }

    return this->functionCall(NodeContext::Expression);
}

inline Node* Parser::functionCall(NodeContext context)
{

    bool        isFunction          = false;
    std::string reference           = "";
    Token&      funcOrRefToken      = peekCurrent();
    std::string funcOrRefIdentifier = funcOrRefToken.literal;
    log_info("DEBUG: %s", funcOrRefIdentifier.c_str());

    if (FunctionResolver::isFunction(funcOrRefIdentifier)) {
        isFunction = true;
    } else if (FunctionResolver::isFunctionAlias(funcOrRefIdentifier)) {
        isFunction          = true;
        funcOrRefIdentifier = FunctionResolver::getFunctionByAlias(funcOrRefIdentifier);
    }

    if (!isFunction) {
        // Check for reference access
        if (peekNext().type == TokenType::Dot) {
            reference = peekCurrent().literal;
            advance();

            std::string& fieldOrFuncIdentifier = peekNext().literal;

            isFunction = false;

            // If the identifier right of the dot is not a function treat it as a reference access
            if (FunctionResolver::isFunction(fieldOrFuncIdentifier)) {
                isFunction = true;
            } else if (FunctionResolver::isFunctionAlias(fieldOrFuncIdentifier)) {
                isFunction            = true;
                fieldOrFuncIdentifier = FunctionResolver::getFunctionByAlias(fieldOrFuncIdentifier);
            }

            if (!isFunction) {
                advance();
                advance();
                return new ReferenceAccess(reference, fieldOrFuncIdentifier, context);
            }
            advance();
            funcOrRefIdentifier = fieldOrFuncIdentifier;
            goto parse_args;
        } else if (context == NodeContext::Expression) {
            return baseType();
        } else {
            error("Unknown function", peekCurrent());
        }
    }

// Plain function call
parse_args:
    advance();
    std::vector<Node*> arguments;

    while (functionCallArgType.count(peekCurrent().type)) {
        arguments.push_back(expression());
        if (peekCurrent().type == TokenType::Comma)
            advance();
    }

    return new FunctionCall(funcOrRefIdentifier, reference, arguments, context);
}

inline Node* Parser::baseType()
{
#ifdef DEBUG
    log_debug("%s", "baseType");
#endif

    if (advanceMatches(baseTypeMatch)) {
        Token& tmp = previous();
        return new LiteralExpr(tmp.literal, tokenTypeToVarType[tmp.type]);
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

inline Node* Parser::statement()
{
    if (advanceMatches(TokenType::If))
        return ifStatement();

    if (advanceMatches(varTypeMatch))
        return declaration();

    if (advanceMatches(TokenType::Return))
        return new ReturnStatement();

    if (advanceMatches(TokenType::Set))
        return assignment();

    return functionCall(NodeContext::Statement);
}

inline Node* Parser::expressionStatement()
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

inline Node* Parser::varDeclaration()
{
    Token& varTypeToken = peekCurrent();

    if (tokenTypeToVarType.count(varTypeToken.type) == 0)
        error("Unknown variable type", varTypeToken);

    Type varType = tokenTypeToVarType[varTypeToken.type];

    advance();
    Token& varName = peekCurrent();
    check(TokenType::Identifier, "Expected variable name");
    check(TokenType::Newline, "Expected newline after declaration");

    return new Variable(varType, varName.literal);
}

inline Node* Parser::assignment()
{
    Token& varOrRefToken = peekCurrent();

    std::string variable;
    std::string reference;

    // Reference access
    if (peekNext().type == TokenType::Dot) {
        reference = varOrRefToken.literal;
        advance();
        advance();
        variable = peekCurrent().literal;

        // Plain var access
    } else {
        variable = varOrRefToken.literal;
    }

    advance();
    check(TokenType::To, "Expected to keyword");
    Node* expr = this->expression();

    return new Assignment(variable, reference, expr);
}

inline Node* Parser::declaration()
{
    return statement();
}

static std::set<TokenType> ifBlockDelimiters   = { TokenType::Elseif, TokenType::Else, TokenType::Endif };
static std::set<TokenType> elseBlockDelimiters = { TokenType::Endif, TokenType::Return };

inline Node* Parser::ifStatement()
{
    Node* condition = expression();
    while (advanceMatches(TokenType::Newline)) { }
    log_debug("%s", TokenEnumToString(peekCurrent().type));

    Node* body = statementBlock(ifBlockDelimiters);

    log_debug("%s", TokenEnumToString(peekCurrent().type));

    std::vector<std::pair<Node*, Node*>> elifs;

    while (peekCurrent().type == TokenType::Elseif) {
        advance();
        Node* elifCondition = expression();
        while (advanceMatches(TokenType::Newline)) { };

        Node* elifBody = statementBlock(ifBlockDelimiters);
        elifs.push_back(std::make_pair(elifCondition, elifBody));
    }
    Node* elseBody = nullptr;

    log_debug("PENIS: %s", TokenEnumToString(peekCurrent().type));

    bool blockEnd = false;

    if (peekCurrent().type == TokenType::Else) {
        advance();
        while (advanceMatches(TokenType::Newline)) { };

        elseBody = statementBlock(elseBlockDelimiters);

        // What the fuck, why
        if (peekCurrent().type == TokenType::Return) {
            blockEnd = true;
            advance();
            while (advanceMatches(TokenType::Newline)) { };

            if (peekCurrent().type == TokenType::Endif) {
                ((StatementBlock*)elseBody)->nodes->emplace_back(new ReturnStatement());
                advance();
            }
        }
    }

    if (!blockEnd)
        check(elseBlockDelimiters, "Expected endif or return");

    return new IfStatement(condition, body, elifs, elseBody);
}

inline Node* Parser::scriptName()
{
    Token& current = peekCurrent();

    if (current.type != TokenType::ScriptName) {
        this->error("Expected ScriptName keyword", current);
    }

    current = advance();

    if (current.type != TokenType::Identifier) {
        this->error("Expected script name", current);
    }

    std::string scriptName = current.literal;
    advance();
    check(TokenType::Newline, "Expected newline");

    return new ScriptNameStatement(scriptName);
};

inline Node* Parser::blocktype()
{
    std::string blocktype;
    std::string blocktypeArg = "";
    Token&      current      = peekCurrent();

    if (current.type != TokenType::Begin) {
        this->error("Expected Begin keyword", current);
    }

    current = advance();

    if (current.type != TokenType::Identifier) {
        this->error("Expected blocktype identifier", current);
    }

    blocktype = current.literal;

    if (Parser::blocktypes.count(blocktype) == 0) {
        this->error("Unknown blocktype", current);
    }

    current = advance();

    if (current.type == TokenType::Identifier) {
        blocktypeArg = current.literal;
        advance();
    }

    return new BlockTypeStatement(blocktype, blocktypeArg);
};

inline Node* Parser::scriptBlock()
{
    std::vector<Node*>* ret     = new std::vector<Node*>();
    Node*               retNode = nullptr;
    Token&              current = peekCurrent();

    try {
        Node* type = blocktype();

        bool hasEndBlock = false;
        current          = advance();

        while (!end()) {

            while (advanceMatches(TokenType::Newline)) { }

            if (peekCurrent().type == TokenType::End) {
                break;
            }

            ret->emplace_back(statement());
        }

        retNode = new ScriptBlock(type, ret);

    } catch (std::runtime_error& e) {
        int size = ret->size();
        for (int i = 0; i < size; i++) {
            delete ret->at(i);
        }
        delete ret;

        throw e;
    }

    return retNode;
}

inline Node* Parser::statementBlock(std::set<TokenType>& delimiters)
{
    std::vector<Node*>* statements = new std::vector<Node*>();
    try {

        while (delimiters.count(peekCurrent().type) == 0) {
            while (advanceMatches(TokenType::Newline)) { }
            statements->emplace_back(statement());
            while (advanceMatches(TokenType::Newline)) { }
        }
    } catch (std::runtime_error& e) {
        int size = statements->size();
        for (int i = 0; i < size; i++) {
            delete statements->at(i);
        }
        delete statements;

        throw e;
    }
    return new StatementBlock(statements);
}
};

#pragma once

#include "compile.hpp"
#include "types/base.hpp"
#include <cassert>
#include <cstdint>
#include <set>

#define DEF_STACK_SIZE 512

namespace Script {

struct Stack {
public:
    ~Stack() {};
    Stack()
        : top(-1)
    {
    }

    void push(Value val)
    {
        // TODO: benchmark scripts and determine optimal stack size
        if (top + 1 < DEF_STACK_SIZE) {
            top++;
            this->buf[top] = val;
        }
    };

    Value pop()
    {
        assert(top != -1);

        Value ret = this->buf[top];
        top       = (top - 1) < -1 ? -1 : top - 1;
        return ret;
    };

private:
    Value   buf[DEF_STACK_SIZE];
    int32_t top;
};

#define VM_STATUS_CODES_ENUM \
    X(SUCCESS)               \
    X(ERROR)
#define X(name) name,

enum class VMStatusCode : uint8_t {
    VM_STATUS_CODES_ENUM
};

#undef X

constexpr const char* VMStatusCodeToString(VMStatusCode e) noexcept
{
#define X(name)                \
    case (VMStatusCode::name): \
        return #name;

    switch (e) {
        VM_STATUS_CODES_ENUM
    }
#undef X
}

class VM {
public:
    VM() {};
    ~VM() {};

    bool execute(CompiledScript* script);

private:
    Stack           stack;
    CompiledScript* script;

    bool handleExpressionCode();
    bool handleOpcode();

    bool evalExpression(uint16_t exprLen);

    bool handleIf();
    bool handleBinaryOperator();
    bool functionCall();
    bool handleAssign();
    bool numberParse();
};

#define ARITHMETIC_OP(left, right, op)                                               \
    do {                                                                             \
        Value _res;                                                                  \
                                                                                     \
        bool _isFloat = ((left).type == Type::Float || (right).type == Type::Float); \
                                                                                     \
        _res.type = _isFloat ? Type::Float : Type::Integer;                          \
                                                                                     \
        if (_isFloat) {                                                              \
            _res.value.f = AS_FLOAT(left) op AS_FLOAT(right);                        \
        } else {                                                                     \
            _res.value.l = AS_INT(left) op AS_INT(right);                            \
        }                                                                            \
                                                                                     \
        this->stack.push(_res);                                                      \
    } while (0)

#define COMPARISON_OP(left, right, op)                \
    do {                                              \
        Value _res;                                   \
                                                      \
        _res.type    = Type::Integer;                 \
        _res.value.l = AS_INT(left) op AS_INT(right); \
                                                      \
        this->stack.push(_res);                       \
    } while (0)
};

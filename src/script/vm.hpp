#pragma once

#include "compile.hpp"
#include <cstdint>

#define DEF_STACK_SIZE 512

namespace Script {

struct Stack {
public:
    ~Stack() {};
    Stack()
        : top(-1)
    {
    }
    void push(uint32_t val)
    {
        // TODO: benchmark scripts and determine optimal stack size
        if (top + 1 < DEF_STACK_SIZE)
            this->buf[top++] = val;
    };
    uint32_t pop()
    {
        uint32_t ret = this->buf[top];
        top          = (top - 1) < -1 ? -1 : top - 1;
        return ret;
    };

private:
    uint32_t buf[DEF_STACK_SIZE];
    uint32_t top;
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
    Stack stack;

    bool handleExpressionCode(CompiledScript* script);
    bool handleOpcode(CompiledScript* script);
};
};

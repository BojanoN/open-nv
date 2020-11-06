#pragma once

#include "context.hpp"
#include "opcode.hpp"
#include "types/base.hpp"
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <vector>

#define DEFAULT_ARR_SIZE (4096)

namespace Script {

class Node;
class BlockTypeStatement;

class CompiledScript {

public:
    CompiledScript()
        : currentSize(0)
        , capacity(DEFAULT_ARR_SIZE)
        , currentReadOffset(0)

    {
        bytecode = (uint8_t*)malloc(DEFAULT_ARR_SIZE);
    }

    ~CompiledScript()
    {
        free(bytecode);
    }

    bool write(uint8_t* source, uint32_t n)
    {
        if ((currentSize + n) > capacity) {
            resize(n);
        }

        memcpy(bytecode + currentSize, source, n);
        currentSize += n;

        return true;
    }

    bool writeAt(uint32_t offset, uint8_t* source, uint32_t n)
    {

        if ((offset + n) > capacity) {
            return false;
        }

        memcpy(bytecode + offset, source, n);

        return true;
    }

    bool writeShortAt(uint32_t offset, uint16_t value)
    {

        if ((offset + sizeof(value)) > capacity) {
            return false;
        }

        *((uint16_t*)&bytecode[offset]) = value;

        return true;
    }

    bool writeZeros(uint32_t n)
    {
        if ((currentSize + n) > capacity) {
            resize(n);
        }

        bzero(this->bytecode + this->currentSize, n);
        currentSize += n;
        return true;
    }

    bool writeByte(uint8_t value)
    {
        if (this->currentSize + 1 < this->capacity) {
            resize(1);
        }

        bytecode[currentSize++] = value;

        return true;
    }

    bool writeShort(uint16_t value)
    {

        if (this->currentSize + sizeof(value) < this->capacity) {
            resize(1);
        }

        *((uint16_t*)&bytecode[currentSize]) = *((uint16_t*)&value);
        currentSize += sizeof(value);

        return true;
    }

    bool writeLong(uint32_t value)
    {

        if (this->currentSize + sizeof(value) < this->capacity) {
            resize(1);
        }

        *((uint32_t*)&bytecode[currentSize]) = *((uint32_t*)&value);
        currentSize += sizeof(value);

        return true;
    }

    bool writeOpcode(Opcode opcode)
    {

        if (this->currentSize + sizeof(Opcode) < this->capacity) {
            resize(1);
        }

        *((uint32_t*)&bytecode[currentSize]) = *((uint32_t*)&opcode);
        currentSize += sizeof(Opcode);

        return true;
    }

    uint8_t peekByte()
    {
        return this->bytecode[this->currentReadOffset];
    }

    uint16_t peekShort()
    {
        if (currentReadOffset + sizeof(uint16_t) > currentSize) {
            return 0;
        }

        return *((uint16_t*)(bytecode + currentReadOffset));
    }

    uint32_t peekLong()
    {
        if (currentReadOffset + sizeof(uint32_t) > currentSize) {
            return 0;
        }

        return *((uint32_t*)(bytecode + currentReadOffset));
    }

    uint32_t readLong()
    {
        if (currentReadOffset + sizeof(uint32_t) < currentSize) {
            uint32_t ret = *((uint32_t*)(bytecode + currentReadOffset));
            currentReadOffset += sizeof(uint32_t);
            return ret;
        } else {
            return 0;
        }
    }

    uint16_t readShort()
    {
        if (currentReadOffset + sizeof(uint16_t) < currentSize) {
            uint16_t ret = *((uint16_t*)(bytecode + currentReadOffset));
            currentReadOffset += sizeof(uint16_t);
            return ret;
        } else {
            return 0;
        }
    }

    uint8_t readByte()
    {
        if (currentReadOffset + sizeof(uint8_t) < currentSize) {

            uint8_t ret = peekByte();
            currentReadOffset++;
            return ret;
        } else {
            return 0;
        }
    }

    double readDouble()
    {
        if (currentReadOffset + sizeof(double) < currentSize) {

            double ret = *((double*)(bytecode + currentReadOffset));
            currentReadOffset += sizeof(double);
            return ret;
        } else {
            return 0;
        }
    }

    bool jump(uint32_t offset)
    {
        if (offset + currentReadOffset > currentSize) {
            return false;
        }

        currentReadOffset += offset;
        return true;
    }

    bool readString(uint32_t len, char* dest)
    {
        if (len + currentReadOffset > currentSize) {
            return false;
        }

        memcpy(dest, bytecode + currentReadOffset, len);
        currentReadOffset += len;

        return true;
    }

    bool isBeforeOffset(uint32_t offset)
    {
        return currentReadOffset < offset;
    }

    bool hasMoreBytes()
    {
        return currentReadOffset < currentSize;
    }

    uint32_t getReadOffset()
    {
        return currentReadOffset;
    }

    uint32_t getSize()
    {
        return currentSize;
    };

    uint8_t readAt(uint32_t i)
    {
        return bytecode[i];
    }

    Value getLocalVariable(uint16_t index)
    {
        Value ret = { Type::Integer, { 0 } };

        // TODO: local var lookup

        return ret;
    }

    void dumpToFile(char* path)
    {
        std::ofstream out { path };

        out.write((char*)bytecode, currentSize);
    }

    void print();

private:
    uint8_t* bytecode;
    uint32_t currentSize;
    uint32_t capacity;

    uint32_t currentReadOffset;

    void resize(int n)
    {
        capacity += DEFAULT_ARR_SIZE * ((n + capacity) / capacity);
        bytecode = (uint8_t*)std::realloc(bytecode, capacity);

        if (bytecode == NULL) {
            throw std::runtime_error("Realloc fail");
        }
    }
};

class Compiler {
public:
    Compiler(GameWorld::GameWorld* store)
        //Compiler(GameWorld::GameData<ESM::Script>* store)
        : ctx(store)
        , nodes(nullptr) {};

    CompiledScript* compile(std::vector<Node*>* ns);

private:
    Context             ctx;
    std::vector<Node*>* nodes;

    int compileNode(Node* node, CompiledScript* out);

    int compileBinaryExpr(Node* node, CompiledScript* out);
    int compileAssignment(Node* node, CompiledScript* out);
    int compileGroupingExpr(Node* node, CompiledScript* out);
    int compileLiteralExpr(Node* node, CompiledScript* out);
    int compileFunctionCall(Node* node, CompiledScript* out);
    int compileScriptBlock(Node* node, CompiledScript* out);

    int compileScriptName(Node* node, CompiledScript* out);
    int compileBlocktype(Node* node, CompiledScript* out);
    int compileExpressionStatement(Node* node, CompiledScript* out);
    int compileIfStatement(Node* node, CompiledScript* out);
    int compileVariable(Node* node, CompiledScript* out);
    int compileReferenceAccess(Node* node, CompiledScript* out);
    int compileStatementBlock(Node* node, CompiledScript* out);
    int compileReturnStatement(Node* node, CompiledScript* out);
    int compileVariableAccess(Node* node, CompiledScript* out);

    // Blocktypes
    int compileOnTrigger(BlockTypeStatement* node, CompiledScript* out, uint32_t blocktypeIDOffset);
    int compileOnTriggerEnter(BlockTypeStatement* node, CompiledScript* out, uint32_t blocktypeIDOffset);
    int compileGameMode(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileMenuMode(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileOnActivate(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileOnAdd(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileOnDeath(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileOnCombatEnd(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileOnDestructionStageChange(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileOnDrop(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);
    int compileOnClose(BlockTypeStatement* blocktype, CompiledScript* out, uint32_t blocktypeCodeOffset);

    int error(std::string cause)
    {
        log_fatal("Compile error: %s ", cause.c_str());

        return -1;
    }
};
}

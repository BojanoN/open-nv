#pragma once
#include "../reader.hpp"
#include <util/strmanip.hpp>
#include <cstdint>
#include <vector>
#include <string>

namespace ESM {

enum ScriptHeaderType : uint16_t {
    OBJECT_SCRIPT = 0x000,
    QUEST_SCRIPT  = 0x010,
    EFFECT_SCRIPT = 0x100
};

enum ScriptHeaderFlags : uint16_t {
    SCRIPT_ENABLED = 1
};

struct __attribute__((packed)) ScriptHeader {
    uint8_t  unusedBytes[4];
    uint32_t refCount;
    uint32_t compiledSize;
    uint32_t variableCount;
    /*
        see ScriptHeaderType enum
    */
    ScriptHeaderType type;
    /*
        see ScriptHeaderFlags enum
    */
    ScriptHeaderFlags flags;
};

enum LocalVariableDataFlags : uint8_t {
    IS_LONG_OR_SHORT = 1
};

struct __attribute__((packed)) LocalVariableData {
    uint32_t index;
    uint8_t  unusedBytes_1[12];
    uint32_t type;
    uint8_t  unusedBytes_2[4];
};

struct __attribute__((packed)) LocalVariable {
    LocalVariableData data;
    std::string       name;
};

enum ReferenceType : uint32_t {
    VARIABLE_REFERENCE,
    OBJECT_REFERENCE
};

struct ScriptReference {
    ReferenceType type;
    uint32_t      reference;
};

struct ScriptData {
    ScriptHeader                 scriptHeader;
    std::vector<uint8_t>         compiledSource;
    std::string                  scriptSource;
    std::vector<LocalVariable>   localVariables;
    std::vector<ScriptReference> references;

    static void load(ESMReader& reader, ScriptData& scriptData);

private:
    const static constexpr ESMType types[] = { ESMType::SCDA, ESMType::SCTX, ESMType::SCRO, ESMType::SCRV, ESMType::SLSD, ESMType::SCVR };
    static bool                    isInCollection(uint32_t type);
};


}; // namespace ESM
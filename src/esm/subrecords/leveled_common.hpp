#pragma once
#include <esm/types.hpp>
#include <cstdint>

namespace ESM {

enum class LVLFFlag : uint8_t {
    FromPlayersLevel   = 0x01,
    ForEachItemInCount = 0x02
};

struct __attribute__((packed)) LVLBaseData {
    int16_t level;
    uint8_t unused[2];
    formid  reference;
    int16_t count;
    uint8_t unused2[2];
};

struct __attribute__((packed)) LVLExtraData {
    formid owner; // NPC_ or FACT
    formid globVarOrRank;
    float  itemCondition;
};

struct LeveledListEntry {
    LVLBaseData  baseData;
    LVLExtraData extraData;
};

};
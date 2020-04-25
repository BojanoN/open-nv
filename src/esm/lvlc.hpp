#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class LVLFFlag : uint8_t {
    FromPlayersLevel   = 0x01,
    ForEachItemInCount = 0x02
};

struct __attribute__((packed)) LVLCBaseData {
    int16_t level;
    uint8_t unused[2];
    formid  reference;
    int16_t count;
    uint8_t unused2[2];
};

struct __attribute__((packed)) LVLCExtraData {
    formid owner; // NPC_ or FACT
    formid globVarOrRank;
    float  itemCondition;
};

struct LeveledList {
    LVLCBaseData  baseData;
    LVLCExtraData extraData;
};

struct LeveledCreature : public Record {
    std::string              editorId;
    ObjectBounds             objectBounds;
    uint8_t                  chanceNone;
    LVLFFlag                 flags;
    std::vector<LeveledList> leveledList;
    ModelData                modelData;

    LeveledCreature(ESMReader& reader);
};
}

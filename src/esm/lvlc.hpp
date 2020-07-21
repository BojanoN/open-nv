#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct LeveledActor : public Record {
    ObjectBounds                  objectBounds;
    uint8_t                       chanceNone;
    LVLFFlag                      flags;
    std::vector<LeveledListEntry> leveledList;
    ModelData                     modelData;

    LeveledActor(ESMReader& reader);
};
}

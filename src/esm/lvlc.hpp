#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
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

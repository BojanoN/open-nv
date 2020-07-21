#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct LeveledItem : public Record {
    ObjectBounds                  objectBounds;
    uint8_t                       chanceNone;
    LVLFFlag                      flags;
    formid                        global; // GLOB
    std::vector<LeveledListEntry> leveledList;
    ModelData                     modelData;

    LeveledItem(ESMReader& reader);
};
}

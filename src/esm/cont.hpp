#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/destructiondata.hpp"
#include "subrecords/item_common.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

enum class ContainerFlags : uint8_t {
    UNKNOWN,
    RESPAWNS
};

struct __attribute__((packed)) ContainerData {
    ContainerFlags flag;
    float          weight;
};

struct Container : public Record {
    ObjectBounds                                    objectBounds;
    std::string                                     name;
    ModelData                                       modelData;
    formid                                          script;
    std::vector<std::pair<ItemData, ItemExtraData>> items;
    DestructionData                                 destructionData;
    ContainerData                                   containerData;
    formid                                          openSound;
    formid                                          closeSound;
    formid                                          loopingSound;

    Container(ESMReader& reader);
};

}

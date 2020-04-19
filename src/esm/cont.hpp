#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

enum class ContainerFlags : uint8_t {
    UNKNOWN,
    RESPAWNS
};

struct __attribute__((packed)) Item {
    formid  item;
    int32_t count;
};

struct __attribute__((packed)) ItemExtraData {
    // FACT or NPC_
    formid owner;
    // GLOB or integer representing rank
    formid requiredRank;
    float  condition;
};

struct __attribute__((packed)) ContainerData {
    ContainerFlags flag;
    float          weight;
};

struct Container : public Record {
    std::string                                 editorId;
    ObjectBounds                                objectBounds;
    std::string                                 name;
    ModelData                                   modelData;
    formid                                      script;
    std::vector<std::pair<Item, ItemExtraData>> items;
    DestructionData                             destructionData;
    ContainerData                               containerData;
    formid                                      openSound;
    formid                                      closeSound;
    formid                                      loopingSound;

    Container(ESMReader& reader);
};

}

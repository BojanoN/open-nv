#pragma once
#include <cstdint>
#include <esm/reader.hpp>
#include <esm/types.hpp>

namespace ESM {

struct BipedalData {
    uint32_t bipedFlags;
    uint8_t  generalFlags;
    uint8_t  padding[3];
};

struct ArmorData {
    int32_t value;
    int32_t maxCondition;
    float   weight;
};

/*
    wut?
*/
struct ArmorAttributes {
    struct ArmorAttributesSmall {
        int16_t ar;
        int16_t flags;
    };
    int16_t  ar;
    int16_t  flags;
    float    dt;
    uint32_t unknown; //12 bytes total

    static void load(ESMReader& reader, ArmorAttributes& attrs);
};

}; // namespace ESM
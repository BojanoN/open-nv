#pragma once
#include <cstdint>


namespace ESM {

enum class ImpactMaterialType : uint32_t {
    STONE,
    DIRT,
    GRASS,
    GLASS,
    METAL,
    WOOD,
    ORGANIC,
    CLOTH,
    WATER,
    HOLLOW_METAL,
    ORGANIC_BUG,
    ORGANIC_GLOW
};

struct __attribute__((packed)) FootstepMaterial {
    uint8_t concSolid[30];
    uint8_t concBroken[30];
    uint8_t metalSolid[30];
    uint8_t metalHollow[30];
    uint8_t metalSheet[30];
    uint8_t wood[30];
    uint8_t sand[30];
    uint8_t dirt[30];
    uint8_t grass[30];
    uint8_t water[30];
};

}; // namespace ESM
#pragma once

#include "reader.hpp"
#include "types.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

namespace ESM {


struct ObjectBounds {
    int16_t x1;
    int16_t y1;
    int16_t z1;
    int16_t x2;
    int16_t y2;
    int16_t z2;
};

};
/*
struct Attributes {
    uint8_t strength;
    uint8_t perception;
    uint8_t endurance;
    uint8_t charisma;
    uint8_t intelligence;
    uint8_t agillity;
    uint8_t luck;
};*/

struct FactRaceRelation {
    formid   faction;
    int32_t  modifier;
    uint32_t groupCombatReaction;
};
/*
struct AlternateTexture {
    uint32_t nameLength;
    char*    name;
    formid   newTexture;
    int32_t  index;
};

struct ModelData {
    std::string filename;
    uint8_t     unused[4];
    //benis
    uint32_t                      alternateTextureCount;
    std::vector<AlternateTexture> alternateTextures;
    uint8_t                       FaceGenModelFlags;

    const static constexpr ESMType filenameType[]    = { ESMType::MODL, ESMType::MOD2, ESMType::MOD3, ESMType::MOD4 };
    const static constexpr ESMType unusedType[]      = { ESMType::MODB, ESMType::NO_ENTRY, ESMType::NO_ENTRY, ESMType::NO_ENTRY };
    const static constexpr ESMType hashesType[]      = { ESMType::MODT, ESMType::MO2T, ESMType::MO3T, ESMType::MO4T };
    const static constexpr ESMType altTexturesType[] = { ESMType::MODS, ESMType::MO2S, ESMType::MO3S, ESMType::MO4S };
    const static constexpr ESMType flagType[]        = { ESMType::MODD, ESMType::NO_ENTRY, ESMType::MOSD, ESMType::NO_ENTRY };

    static void load(ESMReader& reader, ModelData& modelData, int index, ESMType nextSubheader);
};

}*/

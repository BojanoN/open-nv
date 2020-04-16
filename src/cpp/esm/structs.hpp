#pragma once

#include "reader.hpp"
#include "types.hpp"

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace ESM {

struct ObjectBounds {
    int16_t x1;
    int16_t y1;
    int16_t z1;
    int16_t x2;
    int16_t y2;
    int16_t z2;
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

struct AlternateTexture {
    uint32_t    nameLength;
    std::string name;
    formid      newTexture;
    int32_t     index;
};

struct ModelData {
    std::string filename;
    uint8_t     unused[4];
    //benis
    uint32_t                          alternateTextureCount;
    std::vector<std::vector<uint8_t>> textureHashes;
    std::vector<AlternateTexture>     alternateTextures;
    uint8_t                           FaceGenModelFlags;

    static void load(ESMReader& reader, ModelData& modelData, int index, ESMType nextSubheader);
};

};

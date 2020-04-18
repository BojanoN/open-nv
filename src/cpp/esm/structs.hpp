#pragma once

#include "reader.hpp"
#include "types.hpp"

#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_set>
#include <variant>
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
    std::string                   filename;
    uint8_t                       unused[4];
    uint32_t                      alternateTextureCount;
    std::vector<uint8_t>          textureHashes;
    std::vector<AlternateTexture> alternateTextures;
    uint8_t                       FaceGenModelFlags;

    const static constexpr ESMType filenameType[]    = { ESMType::MODL, ESMType::MOD2, ESMType::MOD3, ESMType::MOD4 };
    const static constexpr ESMType unusedType[]      = { ESMType::MODB, ESMType::NO_ENTRY, ESMType::NO_ENTRY, ESMType::NO_ENTRY };
    const static constexpr ESMType hashesType[]      = { ESMType::MODT, ESMType::MO2T, ESMType::MO3T, ESMType::MO4T };
    const static constexpr ESMType altTexturesType[] = { ESMType::MODS, ESMType::MO2S, ESMType::MO3S, ESMType::MO4S };
    const static constexpr ESMType flagType[]        = { ESMType::MODD, ESMType::NO_ENTRY, ESMType::MOSD, ESMType::NO_ENTRY };

    static void load(ESMReader& reader, ModelData& modelData, int index, std::unordered_set<ESMType>& nextSubheaders);
};

struct EffectData {
    uint32_t magnitude;
    uint32_t area;
    uint32_t duration;
    uint32_t type;
    int32_t  actorValue;
};

struct Condition {
    struct ConditionNullReference {
        uint8_t                     type;
        uint8_t                     unused[3];
        std::variant<formid, float> comparisonVal;
        uint32_t                    functionIndex;
        uint32_t                    firstParameter;
        uint32_t                    secondParameter;
        uint32_t                    runOn;
    };

    uint8_t                     type;
    uint8_t                     unused[3];
    std::variant<formid, float> comparisonVal;
    uint32_t                    functionIndex;
    uint32_t                    firstParameter;
    uint32_t                    secondParameter;
    uint32_t                    runOn;
    formid                      reference;

    static void load(ESMReader& reader, Condition& condition);
};

struct Effect {
    formid     baseEffect;
    EffectData data;
    Condition  condition;
};

struct DestructionHeader {
    int32_t health;
    uint8_t count;
    uint8_t flags;
    uint8_t unused[2];
};

struct DestructionStageData {
    uint8_t healthPercentage;
    uint8_t index;
    uint8_t damageStage;
    uint8_t flags;
    int32_t selfDamagePerSecond;
    formid  explosion;
    formid  debris;
    int32_t debrisCount;
};

struct DestructionStage {
    DestructionStageData stageData;
    std::string          modelFilename;
    std::vector<uint8_t> textureHashes;
};

struct DestructionData {
    DestructionHeader             header;
    std::vector<DestructionStage> stages;
    static void                   load(ESMReader& reader, DestructionData& destData, std::unordered_set<ESMType>& nextSubheaders);
};

};

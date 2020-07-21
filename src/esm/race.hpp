#pragma once

#include "record.hpp"
#include "structs.hpp"
#include <unordered_set>

namespace ESM {

class ESMReader;

enum RaceFlags : uint32_t {
    PLAYABLE = 0x1,
    CHILD    = 0x4
};

enum RaceHairColorEnum : uint8_t {
    BLEACHED,
    BROWN,
    CHOCOLATE,
    PLATINUM,
    CORNSILK,
    SUEDE,
    PECAN,
    AUBURN,
    GINGER,
    HONEY,
    GOLD,
    ROSEWOOD,
    BLACK,
    CHESTNUT,
    STEEL,
    CHAMPAGNE
};

enum HeadPartIndex : uint32_t {
    HEAD,
    EARS,
    MOUTH,
    TEETH_HI,
    TEETH_LO,
    TOUNGE,
    LEFT_EYE,
    RIGHT_EYE
};

enum BodyPartIndex : uint32_t {
    UPPER_BODY,
    LEFT_HAND,
    RIGHT_HAND,
    UPPER_BODY_TEXTURE
};

struct SkillBoost {
    int8_t skill;
    int8_t boost;
};

struct RaceData {
    SkillBoost skillBoosts[7];
    uint8_t    unused[2];
    float      maleHeight;
    float      femaleHeight;
    float      maleWeight;
    float      femaleWeight;
    uint32_t   flags;
};

struct RaceVoice {
    formid male;
    formid female;
};

struct RaceDefaultHairStyle {
    formid male;
    formid female;
};

struct RaceDefaultHairColor {
    uint8_t male;
    uint8_t female;
};

struct ModelPart {
    uint32_t    index;
    ModelData   modelData;
    std::string largeIconFilename;
    std::string smallIconFilename;

    static void load(ESMReader& reader, std::vector<ModelPart>& parts, ESMType nextType);
};

struct Race : public Record {

    std::string                   name;
    std::string                   description;
    std::vector<FactRaceRelation> relations;
    RaceData                      raceData;

    formid older;
    formid younger;

    RaceVoice            voices;
    RaceDefaultHairStyle defaultHairStyle;
    RaceDefaultHairColor defaultHairColor;

    float FaceGenMainClamp;
    float FaceGenFaceClamp;

    uint8_t unknown[2];

    std::vector<ModelPart> maleHeadParts;
    std::vector<ModelPart> femaleHeadParts;
    std::vector<ModelPart> maleBodyParts;
    std::vector<ModelPart> femaleBodyParts;

    std::vector<formid> hairs;
    std::vector<formid> eyes;

    std::vector<uint8_t> maleFaceGenGeometry_Symmetric;
    std::vector<uint8_t> maleFaceGenGeometry_Asymmetric;
    std::vector<uint8_t> maleFaceGenTexture_Symmetric;

    std::vector<uint8_t> femaleFaceGenGeometry_Symmetric;
    std::vector<uint8_t> femaleFaceGenGeometry_Asymmetric;
    std::vector<uint8_t> femaleFaceGenTexture_Symmetric;

    Race(ESMReader& reader);
};

};

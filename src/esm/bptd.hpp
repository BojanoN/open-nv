#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class BodyPartNodeFlags : uint8_t {
    SEVERABLE = 0x01,
    IK_DATA = 0x02,
    IK_DATA_BIPED_DATA = 0x04,
    EXPLODABLE = 0x08,
    IK_DATA_IS_HEAD = 0x10,
    IK_DATA_HEADTRACKING = 0x20,
    TO_HIT_CHANCE_ABSOLUTE = 0x40,
};

enum class BodyPartNodeType : uint8_t {
    TORSO,
    HEAD_1,
    HEAD_2,
    LEFT_ARM_1,
    LEFT_ARM_2,
    RIGHT_ARM_1,
    RIGHT_ARM_2,
    LEFT_LEG_1,
    LEFT_LEG_2,
    LEFT_LEG_3,
    RIGHT_LEG_1,
    RIGHT_LEG_2,
    RIGHT_LEG_3,
    BRAIN,
    WEAPON
};

struct BodyPartNodeData {
    float damageMultiplier;
    BodyPartNodeFlags flags;
    BodyPartNodeType type;
    uint8_t healthPercent;
    int8_t  actorValue;
    uint8_t toHitChance;
    uint8_t explosionChance;
    uint16_t explodableDebrisCount;
    formid   explodableDebris; //DEBR
    formid   explodableExplosion; //EXPL
    float    trackingMaxAngle;
    float    explodableDebrisScale;
    int32_t  severableDebrisCount;
    formid   severableDebris; //DEBR
    formid   severableExplosion; //EXPL
    float    severableDebrisScale;
    float    goreTranslateX;
    float    goreTranslateY;
    float    goreTranslateZ;
    float    goreRotateX;
    float    goreRotateY;
    float    goreRotateZ;
    formid   severableImpactDataset; //IPDS
    formid   explodableImpactDataset; //IDPS
    uint8_t  severableDecalCount;
    uint8_t  explodableDecalCount;
    uint8_t  unused[2];
    float    limbReplacementScale;
};


struct BodyPartNode {
    std::string name;
    std::string nodeName;
    std::string VATSTarget;
    std::string ikDataStartNode;
    BodyPartNodeData data;
    std::string limbReplacementModel;
    std::string goreEffectsTargetBone;
    std::vector<uint8_t> textureFileHashes;

    static void load(ESMReader& reader, BodyPartNode& node);
};

struct BodyPartData : public Record {
    ModelData                 modelData;
    std::vector<BodyPartNode> nodes;
    formid                    ragdoll;

    BodyPartData(ESMReader& reader);
};
};
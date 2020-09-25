#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/condition.hpp"
#include "structs.hpp"

namespace ESM {

struct RelatedIdleAnimations {
    formid  parent; //IDLE, or null
    formid  previousSibling; //IDLE, or null
};

enum class AnimationGroupSection : uint8_t {
    IDLE,
    MOVEMENT,
    LEFT_ARM,
    RIGHT_ARM,
    LEFT_HAND,
    WEAPON,
    WEAPON_UP,
    WEAPON_DOWN,
    SPECIAL_IDLE,
    WHOLE_BODY,
    UPPER_BODY
};

enum class IdleAnimationFlags : uint8_t {
    NO_ATTACKING = 0x01
};

struct IdleAnimationData {
    struct IdleAnimationDataSmall {
        AnimationGroupSection  animationGroupSection;
        uint8_t loopingMin;
        uint8_t loopingMax;
        uint8_t unused;
        int16_t replayDelay;
    };

    AnimationGroupSection  animationGroupSection;
    uint8_t loopingMin;
    uint8_t loopingMax;
    uint8_t unused;
    int16_t replayDelay;
    IdleAnimationFlags    flags;
    uint8_t unused_2;

    static void load(ESMReader& reader, IdleAnimationData& data);
};

struct IdleAnimation : public Record {

    ModelData              modelData;
    std::vector<Condition> conditions;
    RelatedIdleAnimations  relatedIdleAnimations;
    IdleAnimationData      data;

    IdleAnimation(ESMReader& reader);
};

};
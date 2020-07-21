#pragma once
#include "record.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

enum class ChallengeType : uint32_t {
    KillFromAFormList,
    KillASpecificFormID,
    KillAnyInACategory,
    HitAnEnemy,
    DiscoverAMapMarker,
    UseAnItem,
    AcquireAnItem,
    UseASkill,
    DoDamage,
    UseAnItemFromAList,
    AcquireAnItemFromAList,
    MiscellaneousStat,
    CraftUsingAnItem,
    ScriptedChallenge,
};

enum class ChallengeFlags : uint32_t {
    StartDisabled    = 1,
    Recurring        = 2,
    ShowZeroProgress = 4
};

struct ChallengeData {
    ChallengeType  type;
    uint32_t       threshold;
    ChallengeFlags flags;
    uint32_t       interval;
    uint16_t       value1;
    uint16_t       value2;
    uint32_t       value3;
};

struct Challenge : public Record {
    std::string   name;
    std::string   pipboyTexture;
    std::string   cornerTexture;
    formid        script;
    std::string   description;
    ChallengeData data;
    formid        value3;
    formid        value4;

    Challenge(ESMReader& reader);
};
}

#pragma once
#include "record.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

struct QuestAndStage {
    formid  quest; // QUST
    uint8_t questStage;
    uint8_t unused[3];
};

struct PerkAbility {
    formid ability; // SPEL
};

enum class EntryPointValues : uint8_t {
    CalculateWeaponDamage         = 0,
    CalculateMyCriticalHitChance  = 1,
    CalculateMyCriticalHitDamage  = 2,
    CalculateWeaponAttackAPCost   = 3,
    CalculateMineExplodeChance    = 4,
    AdjustRangePenalty            = 5,
    AdjustLimbDamage              = 6,
    CalculateWeaponRange          = 7,
    CalculateToHitChance          = 8,
    AdjustExperiencePoints        = 9,
    AdjustGainedSkillPoints       = 10,
    AdjustBookSkillPoints         = 11,
    ModifyRecoveredHealth         = 12,
    CalculateInventoryAPCost      = 13,
    GetDisposition                = 14,
    GetShouldAttack               = 15,
    GetShouldAssist               = 16,
    CalculateBuyPrice             = 17,
    GetBadKarma                   = 18,
    GetGoodKarma                  = 19,
    IgnoreLockedTerminal          = 20,
    AddLeveledListOnDeath         = 21,
    GetMaxCarryWeight             = 22,
    ModifyAddictionChance         = 23,
    ModifyAddictionDuration       = 24,
    ModifyPositiveChemDuration    = 25,
    AdjustDrinkingRadiation       = 26,
    Activate                      = 27,
    MysteriousStranger            = 28,
    HasParalyzingPalm             = 29,
    HackingScienceBonus           = 30,
    IgnoreRunningDuringDetection  = 31,
    IgnoreBrokenLock              = 32,
    HasConcentratedFire           = 33,
    CalculateGunSpread            = 34,
    PlayerKillAPReward            = 35,
    ModifyEnemyCriticalHitChance  = 36,
    ReloadSpeed                   = 37,
    EquipSpeed                    = 38,
    ActionPointRegen              = 39,
    ActionPointCost               = 40,
    MissFortune                   = 41,
    ModifyRunSpeed                = 42,
    ModifyAttackSpeed             = 43,
    ModifyRadiationConsumed       = 44,
    HasPipHacker                  = 45,
    HasMeltdown                   = 46,
    SeeEnemyHealth                = 47,
    HasJuryRigging                = 48,
    ModifyThreatRange             = 49,
    ModifyThread                  = 50,
    HasFastTravelAlways           = 51,
    KnockdownChance               = 52,
    ModifyWeaponStrengthReq       = 53,
    ModifyAimingMoveSpeed         = 54,
    ModifyLightItems              = 55,
    ModifyDamageThresholdDefender = 56,
    ModifyChanceforAmmoItem       = 57,
    ModifyDamageThresholdAttacker = 58,
    ModifyThrowingVelocity        = 59,
    ChanceforItemonFire           = 60,
    HasUnarmedForwardPowerAttack  = 61,
    HasUnarmedBackPowerAttack     = 62,
    HasUnarmedCrouchedPowerAttack = 63,
    HasUnarmedCounterAttack       = 64,
    HasUnarmedLeftPowerAttack     = 65,
    HasUnarmedRightPowerAttack    = 66,
    VATSHelperChance              = 67,
    ModifyItemDamage              = 68,
    HasImprovedDetection          = 69,
    HasImprovedSpotting           = 70,
    HasImprovedItemDetection      = 71,
    AdjustExplosionRadius         = 72,
    Reserved                      = 73,
};

struct PerkEntryPoint {
    EntryPointValues entryPoint;
    uint8_t          function;
    uint8_t          perkConditionTabCount;
};

struct __attribute__((packed)) PerkCondition {
    int8_t    runOn;
    Condition condition;
};

enum class PerkEffectType {
    QuestAndStage,
    Ability,
    EntryPoint
};

struct __attribute__((packed)) PerkEffectHeader {
    PerkEffectType type;
    uint8_t        rank;
    uint8_t        priority;
};

struct __attribute__((packed)) PerkData {
    uint8_t trait; // bool
    uint8_t minLevel;
    uint8_t ranks;
    uint8_t playable; // bool
    uint8_t hidden; // bool
};

struct Perk : public Record {
    std::string editorId;
    std::string name;
    std::string description;
    std::string largeIconFilename;
    std::string smallIconFilename;
    Condition   condition;
    PerkData    data;
};
}

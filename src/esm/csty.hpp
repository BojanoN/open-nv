#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

enum class CombatStyleFlags : uint16_t {
    ChooseAttackUsingChance  = 0x1,
    MeleeAlertOK             = 0x2,
    IgnoreThreats            = 0x10,
    IgnoreDamagingSelf       = 0x20,
    IgnoreDamagingGroup      = 0x40,
    IgnoreDamagingSpectators = 0x80,
    CannotUseStealthboy      = 0x100
};

struct __attribute__((packed)) Standard {
    uint8_t          maneuverDodgeChance;
    uint8_t          maneuverLeftRightChance;
    uint16_t         unused;
    float            maneuverDodgeLRTimerMin;
    float            maneuverDodgeLRTimerMax;
    float            maneuverDodgeForwardTimerMin;
    float            maneuverDodgeForwardTimerMax;
    float            maneuverDodgeBackTimerMin;
    float            maneuverDodgeBackTimerMax;
    float            maneuverDodgeIdleTimerMin;
    float            maneuverDodgeIdleTimerMax;
    uint8_t          meleeDecisionBlockChance;
    uint8_t          meleeDecisionAttackChance;
    uint16_t         unused2;
    float            meleeDecisionRecoilBonusToAttack;
    float            meleeDecisionUnconsciousBonusToAttack;
    float            meleeDecisionHtoHBonusToAttack;
    uint8_t          meleeDecisionPowerAttackChance;
    uint8_t          unused3[3];
    float            meleeDecisionRecoilBonusToPower;
    float            meleeDecisionUnconsciousBonusToPower;
    uint8_t          meleeDecisionPowerAttackNormal;
    uint8_t          meleeDecisionPowerAttackForward;
    uint8_t          meleeDecisionPowerAttackBack;
    uint8_t          meleeDecisionPowerAttackLeft;
    uint8_t          meleeDecisionPowerAttackRight;
    uint8_t          unused4[3];
    float            meleeDecisionHoldTimerMin;
    float            meleeDecisionHoldTimerMax;
    CombatStyleFlags flags;
    uint16_t         unused5;
    uint8_t          maneuverDecisionDodgeChance;
    uint8_t          meleeDecisionRushingAttackChance;
    uint16_t         unused6;
    float            meleeDecisionRushingAttackDistanceMult;
};

struct Advanced {
    float dodgeFatigueModMult;
    float dodgeFatigueModBase;
    float encumbSpeedModBase;
    float encumbSpeedModMult;
    float dodgeWhileUnderAttackMult;
    float dodgeWhileNotUnderAttackMult;
    float dodgeBackWhileUnderAttackMult;
    float dodgeBackWhileNotUnderAttackMult;
    float dodgeForwardWhileUnderAttackMult;
    float dodgeForwardWhileNotUnderAttackMult;
    float blockSkillModMult;
    float blockSkillModBase;
    float blockWhileUnderAttackMult;
    float blockWhileNotUnderAttackMult;
    float attackSkillModMult;
    float attackSkillModBase;
    float attackWhileUnderAttackMult;
    float attackWhileNotUnderAttackMult;
    float attackDuringBlockMult;
    float powerAttFatigueModBase;
    float powerAttFatigueModMult;
};

enum class WeaponRestrictions : uint32_t {
    None,
    MeleeOnly,
    RangedOnly
};

struct Simple {
    float              coverSearchRadius;
    float              takeCoverChance;
    float              waitTimerMin;
    float              waitTimerMax;
    float              waitToFireTimerMin;
    float              waitToFireTimerMax;
    float              fireTimerMin;
    float              fireTimerMax;
    float              rangedWeaponRangeMultMin;
    uint32_t           unused;
    WeaponRestrictions weaponRestrictions;
    float              rangedWeaponRangeMultMax;
    float              maxTargetingFov;
    float              combatRadius;
    float              semiAutoFiringDelayMultMin;
    float              semiAutoFiringDelayMultMax;
};

struct CombatStyle : public Record {
    std::string editorId;
    Standard    standard;
    Advanced    advanced;
    Simple      simple;

    CombatStyle(ESMReader& reader);
};
}

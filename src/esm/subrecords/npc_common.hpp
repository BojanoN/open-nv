#pragma once
#include "material_common.hpp"
#include "sound_common.hpp"
#include "skill_common.hpp"
#include "services_common.hpp"
#include <esm/types.hpp>

#include <cstdint>

namespace ESM {

enum class AIAggression : uint8_t {
    Unaggressive   = 0,
    Aggressive     = 1,
    VeryAggressive = 2,
    Frenzied       = 3,
};

enum class AIConfidence : uint8_t {
    Cowardly  = 0,
    Cautious  = 1,
    Average   = 2,
    Brave     = 3,
    Foolhardy = 4,
};

enum class AIMood : uint8_t {
    Neutral  = 0,
    Afraid   = 1,
    Annoyed  = 2,
    Cocky    = 3,
    Drugged  = 4,
    Pleasant = 5,
    Angry    = 6,
    Sad      = 7,
};

enum class AIAssistance : int8_t {
    NOBODY,
    ALLIES,
    FRIENDS_AND_ALLIES
};

enum class AIAggroRadiusBehavior : uint8_t {
    AggroRadiusBehavior = 0x1
};

struct __attribute__((packed)) AIData {
    AIAggression          aggression;
    AIConfidence          confidence;
    uint8_t               energyLevel;
    uint8_t               responsibility;
    AIMood                mood;
    uint8_t               unused[3];
    Services              services;
    Skills                teaches;
    uint8_t               maximumTrainingLevel;
    AIAssistance          assistance;
    AIAggroRadiusBehavior aggroRadiusBehavior;
    int32_t               aggroRadius;
};

enum class AttackAnimation : uint16_t {
    AttackLeft         = 26,
    AttackLeftUp       = 27,
    AttackLeftDown     = 28,
    AttackLeftIS       = 29,
    AttackLeftISUp     = 30,
    AttackLeftISDown   = 31,
    AttackRight        = 32,
    AttackRightUp      = 33,
    AttackRightDown    = 34,
    AttackRightIS      = 35,
    AttackRightISUp    = 36,
    AttackRightISDown  = 37,
    Attack3            = 38,
    Attack3Up          = 39,
    Attack3Down        = 40,
    Attack3IS          = 41,
    Attack3ISUp        = 42,
    Attack3ISDown      = 43,
    Attack4            = 44,
    Attack4Up          = 45,
    Attack4Down        = 46,
    Attack4IS          = 47,
    Attack4ISUp        = 48,
    Attack4ISDown      = 49,
    Attack5            = 50,
    Attack5Up          = 51,
    Attack5Down        = 52,
    Attack5IS          = 53,
    Attack5ISUp        = 54,
    Attack5ISDown      = 55,
    Attack6            = 56,
    Attack6Up          = 57,
    Attack6Down        = 58,
    Attack6IS          = 59,
    Attack6ISUp        = 60,
    Attack6ISDown      = 61,
    Attack7            = 62,
    Attack7Up          = 63,
    Attack7Down        = 64,
    Attack7IS          = 65,
    Attack7ISUp        = 66,
    Attack7ISDown      = 67,
    Attack8            = 68,
    Attack8Up          = 69,
    Attack8Down        = 70,
    Attack8IS          = 71,
    Attack8ISUp        = 72,
    Attack8ISDown      = 73,
    AttackLoop         = 74,
    AttackLoopUp       = 75,
    AttackLoopDown     = 76,
    AttackLoopIS       = 77,
    AttackLoopISUp     = 78,
    AttackLoopISDown   = 79,
    AttackSpin         = 80,
    AttackSpinUp       = 81,
    AttackSpinDown     = 82,
    AttackSpinIS       = 83,
    AttackSpinISUp     = 84,
    AttackSpinISDown   = 85,
    AttackSpin2        = 86,
    AttackSpin2Up      = 87,
    AttackSpin2Down    = 88,
    AttackSpin2IS      = 89,
    AttackSpin2ISUp    = 90,
    AttackSpin2ISDown  = 91,
    AttackPower        = 92,
    AttackForwardPower = 93,
    AttackBackPower    = 94,
    AttackLeftPower    = 95,
    AttackRightPower   = 96,
    PlaceMine          = 97,
    PlaceMineUp        = 98,
    PlaceMineDown      = 99,
    PlaceMineIS        = 100,
    PlaceMineISUp      = 101,
    PlaceMineISDown    = 102,
    PlaceMine2         = 103,
    PlaceMine2Up       = 104,
    PlaceMine2Down     = 105,
    PlaceMine2IS       = 106,
    PlaceMine2ISUp     = 107,
    PlaceMine2ISDown   = 108,
    AttackThrow        = 109,
    AttackThrowUp      = 110,
    AttackThrowDown    = 111,
    AttackThrowIS      = 112,
    AttackThrowISUp    = 113,
    AttackThrowISDown  = 114,
    AttackThrow2       = 115,
    AttackThrow2Up     = 116,
    AttackThrow2Down   = 117,
    AttackThrow2IS     = 118,
    AttackThrow2ISUp   = 119,
    AttackThrow2ISDown = 120,
    AttackThrow3       = 121,
    AttackThrow3Up     = 122,
    AttackThrow3Down   = 123,
    AttackThrow3IS     = 124,
    AttackThrow3ISUp   = 125,
    AttackThrow3ISDown = 126,
    AttackThrow4       = 127,
    AttackThrow4Up     = 128,
    AttackThrow4Down   = 129,
    AttackThrow4IS     = 130,
    AttackThrow4ISUp   = 131,
    AttackThrow4ISDown = 132,
    AttackThrow5       = 133,
    AttackThrow5Up     = 134,
    AttackThrow5Down   = 135,
    AttackThrow5IS     = 136,
    AttackThrow5ISUp   = 137,
    AttackThrow5ISDown = 138,
    PipBoy             = 167,
    PipBoyChild        = 178,
    Any                = 255,
};


enum class CreatureFlags : uint32_t {
    BIPED                     = 0x00000001,
    ESSENTIAL                 = 0x00000002,
    WEAPON_AND_SHIELD         = 0x00000004,
    RESPAWN                   = 0x00000008,
    SWIMS                     = 0x00000010,
    FLIES                     = 0x00000020,
    WALKS                     = 0x00000040,
    PC_LEVEL_MULT             = 0x00000080,
    NO_LOW_LEVEL_PROCESSING   = 0x00000200,
    NO_BLOOD_SPRAY            = 0x00000800,
    NO_BLOOD_DECAL            = 0x00001000,
    NO_HEAD                   = 0x00008000,
    NO_RIGHT_ARM              = 0x00010000,
    NO_LEFT_ARM               = 0x00020000,
    NO_COMBAT_IN_WATER        = 0x00040000,
    NO_SHADOW                 = 0x00080000,
    NO_VATS_MELEE             = 0x00100000,
    ALLOW_PC_DIALOGUE         = 0x00200000,
    CANT_OPEN_DOORS           = 0x00400000,
    IMMOBILE                  = 0x00800000,
    TILT_FRONT_BACK           = 0x01000000,
    TILT_LEFT_RIGHT           = 0x02000000,
    NO_KNOCKDOWNS             = 0x03000000,
    NOT_PUSHABLE              = 0x08000000,
    ALLOW_PICKPOCKET          = 0x10000000,
    IS_GHOST                  = 0x20000000,
    NO_ROTATING_TO_HEAD_TRACK = 0x40000000,
    INVULNERABLE              = 0x80000000
};

enum class NPCFlags : uint32_t {
    FEMALE                    = 0x00000001,
    ESSENTIAL                 = 0x00000002,
    IS_CHARGEN_FACE_PRESET    = 0x00000004,
    RESPAWN                   = 0x00000008,
    AUTO_CALC_STATS           = 0x00000010,
    PC_LEVEL_MULT             = 0x00000080,
    USE_TEMPLATE              = 0x00000100,
    NO_LOW_LEVEL_PROCESSING   = 0x00000200,
    NO_BLOOD_SPRAY            = 0x00000800,
    NO_BLOOD_DECAL            = 0x00001000,
    NO_VATS_MELEE             = 0x00100000,
    CAN_BE_ALL_RACES          = 0x00400000,
    AUTOCALC_SERVICE          = 0x00800000,
    NO_KNOCKDOWNS             = 0x03000000,
    NOT_PUSHABLE              = 0x08000000,
    NO_ROTATING_TO_HEAD_TRACK = 0x40000000,
};

enum class ACBSTemplateFlags : uint16_t {
    USE_TRAITS            = 0x0001,
    USE_STATS             = 0x0002,
    USE_FACTIONS          = 0x0004,
    USE_ACTOR_EFFECT_LIST = 0x0008,
    USE_AI_DATA           = 0x0010,
    USE_AI_PACKAGES       = 0x0020,
    USE_MODEL_ANIMATION   = 0x0040,
    USE_BASE_DATA         = 0x0080,
    USE_INVENTORY         = 0x0100,
    USE_SCRIPT            = 0x0200
};

struct __attribute__((packed)) NPC_ACBS {
    NPCFlags uint32_t;
    uint16_t fatigue;
    uint16_t barterGold;
    // If the 0x00000080 flag is set, the value is divided by 1000 to give a multiplier.
    int16_t           level;
    uint16_t          calcMin;
    uint16_t          calcMax;
    uint16_t          speedMultiplier;
    float             karma;
    int16_t           dispositionBase;
    ACBSTemplateFlags flag;
};

struct __attribute__((packed)) CREA_ACBS {
    CreatureFlags uint32_t;
    uint16_t      fatigue;
    uint16_t      barterGold;
    // If the 0x00000080 flag is set, the value is divided by 1000 to give a multiplier.
    int16_t           level;
    uint16_t          calcMin;
    uint16_t          calcMax;
    uint16_t          speedMultiplier;
    float             karma;
    int16_t           dispositionBase;
    ACBSTemplateFlags flag;
};

struct __attribute__((packed)) ActorFaction {
    formid  fact;
    uint8_t rank;
    uint8_t unused[3];
};

}; // namespace ESM


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

enum class Skills : int8_t {
    NONE = -1,
    BARTER,
    BIG,
    ENERGY,
    EXPLOSIVES,
    LOCKPICK,
    MEDICINE,
    MELEE,
    REPAIR,
    SCIENCE,
    GUNS,
    SNEAK,
    SPEECH,
    SURVIVAL,
    UNARMED,
};

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

enum class FunctionIndex : uint32_t {
    GetDistance                    = 1,
    GetLocked                      = 5,
    GetPos                         = 6,
    GetAngle                       = 8,
    GetStartingPos                 = 10,
    GetStartingAngle               = 11,
    GetSecondsPassed               = 12,
    GetActorValue                  = 14,
    GetCurrentTime                 = 18,
    GetScale                       = 24,
    IsMoving                       = 25,
    IsTurning                      = 26,
    GetLineOfSight                 = 27,
    GetInSameCell                  = 32,
    GetDisabled                    = 35,
    MenuMode                       = 36,
    GetDisease                     = 39,
    GetVampire                     = 40,
    GetClothingValue               = 41,
    SameFaction                    = 42,
    SameRace                       = 43,
    SameSex                        = 44,
    GetDetected                    = 45,
    GetDead                        = 46,
    GetItemCount                   = 47,
    GetGold                        = 48,
    GetSleeping                    = 49,
    GetTalkedToPC                  = 50,
    GetScriptVariable              = 53,
    GetQuestRunning                = 56,
    GetStage                       = 58,
    GetStageDone                   = 59,
    GetFactionRankDifference       = 60,
    GetAlarmed                     = 61,
    IsRaining                      = 62,
    GetAttacked                    = 63,
    GetIsCreature                  = 64,
    GetLockLevel                   = 65,
    GetShouldAttack                = 66,
    GetInCell                      = 67,
    GetIsClass                     = 68,
    GetIsRace                      = 69,
    GetIsSex                       = 70,
    GetInFaction                   = 71,
    GetIsID                        = 72,
    GetFactionRank                 = 73,
    GetGlobalValue                 = 74,
    IsSnowing                      = 75,
    GetDisposition                 = 76,
    GetRandomPercent               = 77,
    GetQuestVariable               = 79,
    GetLevel                       = 80,
    GetArmorRating                 = 81,
    GetDeadCount                   = 84,
    GetIsAlerted                   = 91,
    GetPlayerControlsDisabled      = 98,
    GetHeadingAngle                = 99,
    IsWeaponOut                    = 101,
    IsTorchOut                     = 102,
    IsShieldOut                    = 103,
    IsFacingUp                     = 106,
    GetKnockedState                = 107,
    GetWeaponAnimType              = 108,
    IsWeaponSkillType              = 109,
    GetCurrentAIPackage            = 110,
    IsWaiting                      = 111,
    IsIdlePlaying                  = 112,
    GetMinorCrimeCount             = 116,
    GetMajorCrimeCount             = 117,
    GetActorAggroRadiusViolated    = 118,
    GetCrime                       = 122,
    IsGreetingPlayer               = 123,
    IsGuard                        = 125,
    HasBeenEaten                   = 127,
    GetFatiguePercentage           = 128,
    GetPCIsClass                   = 129,
    GetPCIsRace                    = 130,
    GetPCIsSex                     = 131,
    GetPCInFaction                 = 132,
    SameFactionAsPC                = 133,
    SameRaceAsPC                   = 134,
    SameSexAsPC                    = 135,
    GetIsReference                 = 136,
    IsTalking                      = 141,
    GetWalkSpeed                   = 142,
    GetCurrentAIProcedure          = 143,
    GetTrespassWarningLevel        = 144,
    IsTrespassing                  = 145,
    IsInMyOwnedCell                = 146,
    GetWindSpeed                   = 147,
    GetCurrentWeatherPercent       = 148,
    GetIsCurrentWeather            = 149,
    IsContinuingPackagePCNear      = 150,
    CanHaveFlames                  = 153,
    HasFlames                      = 154,
    GetOpenState                   = 157,
    GetSitting                     = 159,
    GetFurnitureMarkerID           = 160,
    GetIsCurrentPackage            = 161,
    IsCurrentFurnitureRef          = 162,
    IsCurrentFurnitureObj          = 163,
    GetDayOfWeek                   = 170,
    GetTalkedToPCParam             = 172,
    IsPCSleeping                   = 175,
    IsPCAMurderer                  = 176,
    GetDetectionLevel              = 180,
    GetEquipped                    = 182,
    IsSwimming                     = 185,
    GetAmountSoldStolen            = 190,
    GetIgnoreCrime                 = 192,
    GetPCExpelled                  = 193,
    GetPCFactionMurder             = 195,
    GetPCEnemyofFaction            = 197,
    GetPCFactionAttack             = 199,
    GetDestroyed                   = 203,
    HasMagicEffect                 = 214,
    GetDefaultOpen                 = 215,
    GetAnimAction                  = 219,
    IsSpellTarget                  = 223,
    GetVATSMode                    = 224,
    GetPersuasionNumber            = 225,
    GetSandman                     = 226,
    GetCannibal                    = 227,
    GetIsClassDefault              = 228,
    GetClassDefaultMatch           = 229,
    GetInCellParam                 = 230,
    GetVatsTargetHeight            = 235,
    GetIsGhost                     = 237,
    GetUnconscious                 = 242,
    GetRestrained                  = 244,
    GetIsUsedItem                  = 246,
    GetIsUsedItemType              = 247,
    GetIsPlayableRace              = 254,
    GetOffersServicesNow           = 255,
    GetUsedItemLevel               = 258,
    GetUsedItemActivate            = 259,
    GetBarterGold                  = 264,
    IsTimePassing                  = 265,
    IsPleasant                     = 266,
    IsCloudy                       = 267,
    GetArmorRatingUpperBody        = 274,
    GetBaseActorValue              = 277,
    IsOwner                        = 278,
    IsCellOwner                    = 280,
    IsHorseStolen                  = 282,
    IsLeftUp                       = 285,
    IsSneaking                     = 286,
    IsRunning                      = 287,
    GetFriendHit                   = 288,
    IsInCombat                     = 289,
    IsInInterior                   = 300,
    IsWaterObject                  = 304,
    IsActorUsingATorch             = 306,
    IsXBox                         = 309,
    GetInWorldspace                = 310,
    GetPCMiscStat                  = 312,
    IsActorEvil                    = 313,
    IsActorAVictim                 = 314,
    GetTotalPersuasionNumber       = 315,
    GetIdleDoneOnce                = 318,
    GetNoRumors                    = 320,
    WhichServiceMenu               = 323,
    IsRidingHorse                  = 327,
    IsInDangerousWater             = 332,
    GetIgnoreFriendlyHits          = 338,
    IsPlayersLastRiddenHorse       = 339,
    IsActor                        = 353,
    IsEssential                    = 354,
    IsPlayerMovingIntoNewSpace     = 358,
    GetTimeDead                    = 361,
    GetPlayerHasLastRiddenHorse    = 362,
    IsChild                        = 365,
    GetLastPlayerAction            = 367,
    IsPlayerActionActive           = 368,
    IsTalkingActivatorActor        = 370,
    IsInList                       = 372,
    GetHasNote                     = 382,
    GetHitLocation                 = 391,
    IsPC1stPerson                  = 392,
    GetCauseofDeath                = 397,
    IsLimbGone                     = 398,
    IsWeaponInList                 = 399,
    HasFriendDisposition           = 403,
    GetVATSValue                   = 408,
    IsKiller                       = 409,
    IsKillerObject                 = 410,
    GetFactionCombatReaction       = 411,
    Exists                         = 415,
    GetGroupMemberCount            = 416,
    GetGroupTargetCount            = 417,
    GetObjectiveCompleted          = 420,
    GetObjectiveDisplayed          = 421,
    GetIsVoiceType                 = 427,
    GetPlantedExplosive            = 428,
    IsActorTalkingThroughActivator = 430,
    GetHealthPercentage            = 431,
    GetIsObjectType                = 433,
    GetDialogueEmotion             = 435,
    GetDialogueEmotionValue        = 436,
    GetIsCreatureType              = 438,
    GetInZone                      = 446,
    HasPerk                        = 449,
    GetFactionRelation             = 450,
    IsLastIdlePlayed               = 451,
    GetPlayerTeammate              = 454,
    GetPlayerTeammateCount         = 455,
    GetActorCrimePlayerEnemy       = 459,
    GetActorFactionPlayerEnemy     = 460,
    IsPlayerTagSkill               = 462,
    IsPlayerGrabbedRef             = 464,
    GetDestructionStage            = 471,
    GetIsAlignment                 = 474,
    GetThreatRatio                 = 478,
    GetIsUsedItemEquipType         = 480,
    GetConcussed                   = 489,
    GetMapMarkerVisible            = 492,
    GetPermanentActorValue         = 495,
    GetKillingBlowLimb             = 496,
    GetWeaponHealthPerc            = 500,
    GetRadiationLevel              = 503,
    GetLastHitCritical             = 510,
    IsCombatTarget                 = 515,
    GetVATSRightAreaFree           = 518,
    GetVATSLeftAreaFree            = 519,
    GetVATSBackAreaFree            = 520,
    GetVATSFrontAreaFree           = 521,
    GetIsLockBroken                = 522,
    IsPS3                          = 523,
    IsWin32                        = 524,
    GetVATSRightTargetVisible      = 525,
    GetVATSLeftTargetVisible       = 526,
    GetVATSBackTargetVisible       = 527,
    GetVATSFrontTargetVisible      = 528,
    IsInCriticalStage              = 531,
    GetXPForNextLevel              = 533,
    GetQuestCompleted              = 546,
    IsGoreDisabled                 = 550,
    GetSpellUsageNum               = 555,
    GetActorsInHigh                = 557,
    HasLoaded3D                    = 558,
    GetReputation                  = 573,
    GetReputationPct               = 574,
    GetReputationThreshold         = 575,
    IsHardcore                     = 586,
    GetForceHitReaction            = 601,
    ChallengeLocked                = 607,
    GetCasinoWinningStage          = 610,
    PlayerInRegion                 = 612,
    GetChallengeCompleted          = 614,
    IsAlwaysHardcore               = 619,
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
    FunctionIndex               functionIndex;
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

struct BipedalData {
    uint32_t bipedFlags;
    uint8_t  generalFlags;
    uint8_t  padding[3];
};

struct ArmorData {
    int32_t value;
    int32_t maxCondition;
    float   weight;
};

/*
    wut?
*/
struct ArmorAttributes {
    struct ArmorAttributesSmall {
        int16_t ar;
        int16_t flags;
    };
    int16_t  ar;
    int16_t  flags;
    float    dt;
    uint32_t unknown; //12 bytes total

    static void load(ESMReader& reader, ArmorAttributes& attrs);
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

enum class AttackAnimations : uint16_t {
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

struct __attribute__((packed)) Item {
    formid  item;
    int32_t count;
};

struct __attribute__((packed)) ItemExtraData {
    // FACT or NPC_
    formid owner;
    // GLOB or integer representing rank
    formid requiredRank;
    float  condition;
};

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

enum class Services : uint32_t {
    Weapons       = 0x00000001,
    Armor         = 0x00000002,
    Alcohol       = 0x00000004,
    Books         = 0x00000008,
    Food          = 0x00000010,
    Chems         = 0x00000020,
    Stimpacks     = 0x00000040,
    Lights        = 0x00000080,
    Miscellaneous = 0x00000400,
    Potions       = 0x00002000,
    Training      = 0x00004000,
    Recharge      = 0x00010000,
    Repair        = 0x00020000,
};

enum class AIAssistance : int8_t {
    NOBODY,
    ALLIES,
    FRIENDS_AND_ALLIES
};

enum class AIAggroRadiusBehavior : uint8_t {
    AggroRadiusBehavior = 0x1
};

struct __attribute__((packed)) AIDT {
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

};

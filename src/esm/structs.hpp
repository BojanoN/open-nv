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

    static void load(ESMReader& reader, ModelData& modelData, int index);

private:
    const static constexpr ESMType filenameType[]    = { ESMType::MODL, ESMType::MOD2, ESMType::MOD3, ESMType::MOD4 };
    const static constexpr ESMType unusedType[]      = { ESMType::MODB, ESMType::NO_ENTRY, ESMType::NO_ENTRY, ESMType::NO_ENTRY };
    const static constexpr ESMType hashesType[]      = { ESMType::MODT, ESMType::MO2T, ESMType::MO3T, ESMType::MO4T };
    const static constexpr ESMType altTexturesType[] = { ESMType::MODS, ESMType::MO2S, ESMType::MO3S, ESMType::MO4S };
    const static constexpr ESMType flagType[]        = { ESMType::MODD, ESMType::NO_ENTRY, ESMType::MOSD, ESMType::NO_ENTRY };

    const static constexpr ESMType index_0[] = {ESMType::MODL,ESMType::MODB, ESMType::MODT, ESMType::MODS,ESMType::MODD};
    const static constexpr ESMType index_1[] = {ESMType::MOD2, ESMType::MO2T, ESMType::MO2S};
    const static constexpr ESMType index_2[] = {ESMType::MOD3, ESMType::MO3T, ESMType::MO3S, ESMType::MOSD};
    const static constexpr ESMType index_3[] = {ESMType::MOD4, ESMType::MO4T, ESMType::MO4S};    

    const static constexpr ESMType* types[4] = {index_0, index_1, index_2, index_3};
    static bool isInCollection(uint32_t type, int index);
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
    static void                   load(ESMReader& reader, DestructionData& destData);

private:
    const static constexpr ESMType types[] = {ESMType::DSTD, ESMType::DMDL, ESMType::DMDT, ESMType::DSTF};
    static bool isInCollection(uint32_t type);
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

enum class SoundLevel : uint32_t {
    LOUD,
    NORMAL,
    SILENT
};

};

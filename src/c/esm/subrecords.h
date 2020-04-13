#pragma once

#include "esm.h"

typedef struct __attribute__((packed)) {
    Subrecord base;
    float     version;
    uint32_t  numRecords;
    uint32_t  nextObjectId;
} HEDR;

typedef struct __attribute__((packed)) {
    Subrecord base;
    sds       author;
} CNAM;

typedef struct __attribute__((packed)) {
    int16_t x1;
    int16_t y1;
    int16_t z1;
    int16_t x2;
    int16_t y2;
    int16_t z2;
} ObjectBounds;
/*
  Decal data
*/
typedef struct __attribute__((packed)) {
    float   minWidth;
    float   maxWidth;
    float   minHeight;
    float   maxHeight;
    float   depth;
    float   shininess;
    float   parallaxScale;
    uint8_t parallaxPasses;
    uint8_t flags;
    uint8_t unused[2];
    uint8_t color[4];
} DecalData;

/*
 * NPC stats
 */
typedef struct __attribute__((packed)) {
    // 4 skill total
    int32_t  tagSkills[4];
    uint32_t flags;
    uint32_t services;
    int8_t   teaches;
    uint8_t  maxTrainingLevel;
    uint8_t  unused[2];
} ClassData;

/*
 * Attributes
 */
typedef struct __attribute__((packed)) {
    uint8_t strength;
    uint8_t perception;
    uint8_t endurance;
    uint8_t charisma;
    uint8_t intelligence;
    uint8_t agility;
    uint8_t luck;
} ClassAttributes;

typedef struct __attribute__((packed)) {
    formid  faction;
    int32_t modifier;

    /*
    0 - NEUTRAL
    1 - ENEMY
    2 - ALLY
    3 - FRIEND
  */
    uint32_t groupCombatReaction;
} FactionRaceEx;

typedef struct __attribute__((packed)) {
    /*
    0x01 - hidden
    0x02 - evil
    0x04 - special combat
    */
    uint8_t flags_1;
    /*
    0x01 - track crime
    0x02 - allow sell
    */
    uint8_t flags_2;
    uint8_t unused[2];
} FactionData;

typedef struct __attribute__((packed)) {
    int32_t  rankNumber;
    sds      male;
    sds      female;
    uint32_t insignia;
} FactionRank;

typedef struct {
    sds name;
    /*
     * FormID of a TXST record.
     */
    formid   newTexture;
    uint32_t index;
} AlternateTexture;

typedef struct __attribute__((packed)) {
    Subrecord base;
    /*
     * Array of filenames.
     * Matches subrecords MODL, MOD2, MOD3, MOD4
     */
    sds     filenames[4];
    uint8_t noFname;
    /*
     * MODB
     */
    uint32_t unknown;
    /*
     * MODT, MO2T, MO3T, MO4T
     */
    uint8_t** textureHashes;
    /*
     * MODS, MO2S, MO3S, MO4S
     */
    AlternateTexture* alternateTextures;
    /*
     * MODD, MOSD
     */
    uint8_t MODDFlags[2];
} ModelDataSubrecord;

typedef struct __attribute__((packed)) {
    uint8_t skill;
    uint8_t boost;
} SkillBoost;

typedef struct __attribute__((packed)) {
    /*
     * FormID of a VTYP record.
     */
    formid male;
    formid female;
} RaceVoices;

typedef struct __attribute__((packed)) {
    /*
     * FormID of a HAIR record. Can be null.
     */
    formid male;
    formid female;
} RaceDefaultHairStyle;

typedef struct __attribute__((packed)) {
    /*
     * See RaceHairColorEnum
     */
    uint8_t male;
    uint8_t female;
} RaceDefaultHairColor;

typedef struct __attribute__((packed)) {
    SkillBoost skillBoosts[7];
    uint8_t    padding[2];
    float      maleHeight;
    float      femaleHeight;
    float      maleWeight;
    float      femaleWeight;
    uint32_t   flags;
} RaceData;

typedef struct __attribute__((packed)) {
    Subrecord base;
    /*
     * See HeadPartIndex and BodyPartIndex enums.
     */
    uint32_t            index;
    ModelDataSubrecord* modelData;
    sds                 largeIcon;
    sds                 smallIcon;
} ModelPart;

typedef enum {
    PLAYABLE = 0x1,
    CHILD    = 0x4
} RaceFlags;

typedef enum {
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
} RaceHairColorEnum;

typedef enum {
    HEAD,
    EARS,
    MOUTH,
    TEETH_HI,
    TEETH_LO,
    TOUNGE,
    LEFT_EYE,
    RIGHT_EYE
} HeadPartIndex;

typedef enum {
    UPPER_BODY,
    LEFT_HAND,
    RIGHT_HAND,
    UPPER_BODY_TEXTURE
} BodyPartIndex;

typedef enum {
    RANDOM_FREQUENCY_SHIFT   = 0x1,
    PLAY_AT_RANDOM           = 0x2,
    ENVIRONMENT_IGNORED      = 0x4,
    RANDOM_LOCATION          = 0x8,
    LOOP                     = 0x10,
    MENU_SOUND               = 0x20,
    TWO_DIMENSIONAL          = 0x40,
    THREE_SIXTY_LFE          = 0x80,
    DIALOGUE_SOUND           = 0x100,
    ENVELOPE_FAST            = 0x200,
    ENVELOPE_SLOW            = 0x400,
    TWO_DIMENSIONAL_RADIUS   = 0x800,
    MUTE_WHEN_SUBMERGED      = 0x1000,
    START_AT_RANDOM_POSITION = 0x2000
} SoundFlags;

/*
    SNDX
*/
typedef struct {
    uint8_t  minimumAttenuationDistance; //multiply by 5
    uint8_t  maximumAttenuationDistance; //multiply by 100
    int8_t   frequencyAdjustmentPercentage;
    uint8_t  unused;
    uint32_t flags;
    int16_t  staticAttenuationCdB;
    uint8_t  stopTime;
    uint8_t  startTime;
} SoundEx;

/*
    SNDD
*/
typedef struct {
    uint8_t  minimumAttenuationDistance; //multiply by 5
    uint8_t  maximumAttenuationDistance; //multiply by 100
    int8_t   frequencyAdjustmentPercentage;
    uint8_t  unused;
    uint32_t flags;
    int16_t  staticAttenuationCdB;
    uint8_t  stopTime;
    uint8_t  startTime;
    int16_t  attenuationPoints[5];
    int16_t  reverbAttenuationControl;
    int32_t  priority;
    int32_t  x;
    int32_t  y;
} SoundData;

typedef enum {
    HOSTILE,
    RECOVER,
    DETRIMENTAL,
    UNKNOWN_MGEF_FLAG_0,
    SELF,
    TOUCH,
    TARGET,
    NO_DURATION,
    NO_MAGNITUDE,
    NO_AREA,
    FX_PERSIST,
    UNKNOWN_MGEF_FLAG_1,
    GORY_VISUALS,
    DISPLAY_NAME_ONLY,
    UNKNOWN_MGEF_FLAG_2,
    RADIO_BROADCAST,
    UNKNOWN_MGEF_FLAG_3,
    UNKNOWN_MGEF_FLAG_4,
    UNKNOWN_MGEF_FLAG_5,
    USE_SKILL,
    USE_ATTRIBUTE,
    UNKNOWN_MGEF_FLAG_6,
    UNKNOWN_MGEF_FLAG_7,
    UNKNOWN_MGEF_FLAG_8,
    PAINLESS,
    SPRAY_PROJECTILE_TYPE, //If both spray and bolt are 1, then Fog type
    BOLT_PROJECTILE_TYPE,
    NO_HIT_EFFECT,
    NO_DEATH_DISPEL,
    UNKNOWN_MGEF_FLAG_9
} MagicEffectDataFlags;

typedef enum {
    VALUE_MODIFIER,
    SCRIPT,
    DISPEL,
    CURE_DISEASE,
    UNKNOWN_MGEF_ARCHTYPE_0,
    UNKNOWN_MGEF_ARCHTYPE_1,
    UNKNOWN_MGEF_ARCHTYPE_2,
    UNKNOWN_MGEF_ARCHTYPE_3,
    UNKNOWN_MGEF_ARCHTYPE_4,
    UNKNOWN_MGEF_ARCHTYPE_5,
    UNKNOWN_MGEF_ARCHTYPE_6,
    INVISIBILITY_MAGIC_EFFECT,
    CHAMELEON_MAGIC_EFFECT,
    LIGHT,
    UNKNOWN_MGEF_ARCHTYPE_7,
    UNKNOWN_MGEF_ARCHTYPE_8,
    LOCK,
    OPEN,
    BOUND_ITEM,
    SUMMON_CREATURE,
    UNKNOWN_MGEF_ARCHTYPE_9,
    UNKNOWN_MGEF_ARCHTYPE_10,
    UNKNOWN_MGEF_ARCHTYPE_11,
    UNKNOWN_MGEF_ARCHTYPE_12,
    PARALYSIS_MAGIC_EFFECT,
    UNKNOWN_MGEF_ARCHTYPE_13,
    UNKNOWN_MGEF_ARCHTYPE_14,
    UNKNOWN_MGEF_ARCHTYPE_15,
    UNKNOWN_MGEF_ARCHTYPE_16,
    UNKNOWN_MGEF_ARCHTYPE_17,
    CURE_PARALYSIS,
    CURE_ADDICTION,
    CURE_POISON,
    CONCUSSION,
    VALUE_AND_PARTS,
    LIMB_CONDITION,
    TURBO_MAGIC_EFFECT
} MagicEffectArchtype;

typedef enum {
    NONE_ACTOR_VALUE = -1,
    AGGRESION,
    CONFIDENCE,
    ENERGY,
    RESPONSIBILITY,
    MOOD,
    STRENGTH,
    PERCEPTION,
    ENDURANCE,
    CHARSIMA,
    INTELLIGENCE,
    AGILITY,
    LUCK,
    ACTION_POINTS,
    CARRY_WEIGHT,
    CRITICAL_CHANCE,
    HEAL_RATE,
    HEALTH,
    MELEE_DAMAGE,
    DAMAGE_RESISTANCE,
    POISON_RESISTANCE,
    RAD_RESISTANCE,
    SPEED_MULTIPLIER,
    FATIGUE,
    KARMA,
    XP,
    PERCEPTION_CONDITION,
    ENDURANCE_CONDITION,
    LEFT_ATTACK_CONDITION,
    RIGHT_ATTACK_CONDITION,
    LEFT_MOBILITY_CONDITION,
    RIGHT_MOBILITY_CONDITION,
    BRAIN_CONDITION,
    BARTER,
    BIG_GUNS,
    ENERGY_WEAPONS,
    EXPLOSIVES,
    LOCKPICK,
    MEDICINE,
    MELEE_WEAPONS,
    REPAIR,
    SCIENCE,
    GUNS,
    SNEAK,
    SPEECH,
    SURVIVAL,
    UNARMED,
    INVENTORY_WEIGHT,
    PARALYSIS_ACTOR_VALUE,
    INVISIBILITY_ACTOR_VALUE,
    CHAMELEON_ACTOR_VALUE,
    NIGHT_EYE,
    TURBO_ACTOR_VALUE,
    FIRE_RESISTANCE,
    WATER_BREATHING,
    RAD_LEVEL,
    BLOODY_MESS,
    UNARMED_DAMAGE,
    ASSISTANCE,
    ELECTRIC_RESISTANCE,
    FROST_RESISTANCE,
    ENERGY_RESISTANCE,
    EMP_RESISTANCE,
    UNKNOWN_ACTOR_VALUE_0,
    UNKNOWN_ACTOR_VALUE_1,
    UNKNOWN_ACTOR_VALUE_2,
    UNKNOWN_ACTOR_VALUE_3,
    UNKNOWN_ACTOR_VALUE_4,
    UNKNOWN_ACTOR_VALUE_5,
    UNKNOWN_ACTOR_VALUE_6,
    UNKNOWN_ACTOR_VALUE_7,
    UNKNOWN_ACTOR_VALUE_8,
    UNKNOWN_ACTOR_VALUE_9,
    IGNORE_CRIPPLED_LIMBS,
    DEHYDRATION,
    HUNGER,
    SLEEP_DEPRIVATION,
    DAMAGE
} ActorValue;

/*
    DATA (MGEF)
*/
typedef struct {
    /*
        See MagicEffectDataFlags enum
    */
    uint32_t flags;
    float    baseCost; //unused
    formid   associatedItem;
    int32_t  magicSchool; //always -1
    /*
        see ActorValue enum
    */
    int32_t  resistanceType;
    uint16_t unused_1;
    uint8_t  unused_2[2];
    formid   light; //LIGH
    float    projectileSpeed;
    formid   effectShader; //EFSH
    formid   objectDisplayShader; //EFSH
    formid   effectSound; //SOUN
    formid   boldSound; //SOUN
    formid   hitSound; //SOUN
    formid   areaSound; //SOUN
    float    constantEffectEnchantmentFactor; //unused
    float    constantEffectBarterFactor; //unused
    /*
        see MagicEffectArchtype enum
    */
    uint32_t archtype;
    /*
        see ActorValue enum
    */
    int32_t actorValue;
} MagicEffectData;

typedef enum {
    OBJECT_SCRIPT = 0x000,
    QUEST_SCRIPT  = 0x010,
    EFFECT_SCRIPT = 0x100
} ScriptHeaderType;

typedef enum {
    SCRIPT_ENABLED = 1
} ScriptHeaderFlags;

typedef struct __attribute__((packed)) {
    uint8_t  unusedBytes[4];
    uint32_t refCount;
    uint32_t compiledSize;
    uint32_t variableCount;
    /*
        see ScriptHeaderType enum
    */
    uint16_t type;
    /*
        see ScriptHeaderFlags enum
    */
    uint16_t flags;
} ScriptHeader;

typedef enum {
    IS_LONG_OR_SHORT = 1
} LocalVariableDataFlags;

typedef struct __attribute__((packed)) {
    uint32_t index;
    uint8_t  unusedBytes_1[12];
    uint8_t  flags;
    uint8_t  unusedBytes_2[7];
} LocalVariableData;

typedef struct __attribute__((packed)) {
    LocalVariableData data;
    sds               name;
} LocalVariable;

typedef enum {
    VARIABLE_REFERENCE,
    OBJECT_REFERENCE
} ReferenceType;

typedef struct __attribute__((packed)) {
    ReferenceType type;
    union {
        uint32_t variableReference;
        formid   objectReference;
    } referenceValue;
} ScriptReference;

typedef struct __attribute__((packed)) {
    uint8_t materialType;
    uint8_t friction;
    uint8_t restitution;
} HavokData;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint8_t  unused[8];
    uint8_t  flags;
    uint8_t  padding[3];
} ENCH_enit;

typedef struct __attribute__((packed)) {
    uint32_t magnitude;
    uint32_t area;
    uint32_t duration;
    uint32_t type;
    uint32_t actorValues;
} ENCH_efit;

typedef struct __attribute__((packed)) {
    uint8_t type;
    uint8_t unused[3];
    union {
        formid comparisonFormid;
        float  comparisonFloat;
    };

} CTDA;

sds        init_cstring_subrecord(FILE* esm_file, SubrecordHeader* subrecordHead, const char* loggingName);
ModelPart* init_ModelPartCollection(FILE* esm_file);
void       free_ModelPartCollection(ModelPart* collection);

void log_ObjectBounds(ObjectBounds*);
void log_DecalData(DecalData*);
void log_FactionRaceEx(FactionRaceEx*);
void log_FactionData(FactionData*);
void log_ClassData(ClassData*);
void log_ClassAttributes(ClassAttributes*);
void log_RaceData(RaceData* subrecord);
void log_RaceVoices(RaceVoices* voices);
void log_RaceDefaultHairStyle(RaceDefaultHairStyle* subrecord);
void log_RaceDefaultHairColor(RaceDefaultHairColor* subrecord);
void log_SNDD(SoundData*);
void log_SoundData(SoundData*);
void log_SoundEx(SoundData*);
void log_MagicEffectData(MagicEffectData*);
void log_ScriptHeader(ScriptHeader*);
void log_LocalVariableData(LocalVariableData*);
void log_HavokData(HavokData*);

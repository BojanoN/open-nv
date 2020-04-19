#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {
class ESMReader;

enum MagicEffectDataFlags : uint32_t {
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
};

enum MagicEffectArchtype : uint32_t {
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
};

enum ActorValue : int32_t {
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
};

/*
    DATA (MGEF)
*/
struct __attribute__((packed)) MagicEffectData {
    /*
        See MagicEffectDataFlags enum
    */
    MagicEffectDataFlags flags;
    float                baseCost; //unused
    formid               associatedItem;
    int32_t              magicSchool; //always -1
    /*
        see ActorValue enum
    */
    ActorValue resistanceType;
    uint16_t   unused_1;
    uint8_t    unused_2[2];
    formid     light; //LIGH
    float      projectileSpeed;
    formid     effectShader; //EFSH
    formid     objectDisplayShader; //EFSH
    formid     effectSound; //SOUN
    formid     boldSound; //SOUN
    formid     hitSound; //SOUN
    formid     areaSound; //SOUN
    float      constantEffectEnchantmentFactor; //unused
    float      constantEffectBarterFactor; //unused
    /*
        see MagicEffectArchtype enum
    */
    MagicEffectArchtype archtype;
    /*
        see ActorValue enum
    */
    ActorValue actorValue;
};

struct MagicEffect : public Record {
    std::string            editorId;
    std::string            name;
    std::string            description;
    std::string            largeIconFilename;
    std::string            smallIconFilename;
    std::vector<ModelData> modelData;
    MagicEffectData        magicEffectData;

    MagicEffect(ESMReader& reader);
};

}

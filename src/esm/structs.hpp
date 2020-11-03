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

struct __attribute__((packed)) rgb {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct rgbf {
    float red;
    float green;
    float blue;
};

struct rgba {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

struct OwnershipData {
    formid  owner; //FACT, ACHR, CREA, NPC_
    int32_t factionRank;
};

struct Vertex {
    float x;
    float y;
    float z;
};

struct Vertex8 {
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

enum class TriangleFlags : uint32_t {
    Triangle0External = 0x001,
    Triangle1External = 0x002,
    Triangle2External = 0x004,
    PreferredPathing  = 0x040,
    Water             = 0x200,
    ContainsDoor      = 0x400,
};

struct Triangle {
    int16_t vertex1;
    int16_t vertex2;
    int16_t vertex3;

    int16_t       edgeVertex12;
    int16_t       edgeVertex23;
    int16_t       edgeVertex31;
    TriangleFlags flags;
};
enum class ActorValue : int32_t {
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

struct __attribute__((packed)) ItemQuality {
    int32_t value;
    float   weight;
};

enum class WaterRenderType : uint32_t {
    Reflection = 1,
    Refraction = 2
};

struct WaterRenderData {
    formid          reference;
    WaterRenderType type;
};
};

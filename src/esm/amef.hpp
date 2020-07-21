#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

enum class AmmoEffectType : uint32_t {
    DamageMod,
    DRMod,
    DTMod,
    SpreadMod,
    WeaponConditionMod,
    FatigueMod
};

enum class Operation : uint32_t {
    Add,
    Multiply,
    Subtract
};

struct AmmoEffectData {
    AmmoEffectType type;
    Operation      operation;
    float          value;
};

struct AmmoEffect : public Record {
    std::string    name;
    AmmoEffectData data;

    AmmoEffect(ESMReader& reader);
};
}

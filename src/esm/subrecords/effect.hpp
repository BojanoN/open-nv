#pragma once
#include "condition.hpp"
#include <esm/types.hpp>
#include <cstdint>

namespace ESM {

struct EffectData {
    uint32_t magnitude;
    uint32_t area;
    uint32_t duration;
    uint32_t type;
    int32_t  actorValue;
};



struct Effect {
    formid     baseEffect;
    EffectData data;
    Condition  condition;
};

};
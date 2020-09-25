#pragma once
#include <esm/types.hpp>
#include <cstdint>

namespace ESM {

struct FactRaceRelation {
    formid   faction;
    int32_t  modifier;
    uint32_t groupCombatReaction;
};

};

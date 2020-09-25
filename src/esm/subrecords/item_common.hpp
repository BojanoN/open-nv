#pragma once
#include <esm/types.hpp>
#include <cstdint>

namespace ESM {

struct __attribute__((packed)) ItemData {
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

};
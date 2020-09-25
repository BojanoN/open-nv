#pragma once
#include "condition.hpp"
#include <esm/types.hpp>
#include <cstdint>

namespace ESM {

enum class EquipmentType : int32_t {
    NONE     = -1,
    BIG      = 0,
    ENERGY   = 1,
    SMALL    = 2,
    MELEE    = 3,
    UNARMED  = 4,
    THROWN   = 5,
    MINE     = 6,
    BODY     = 7,
    HEAD     = 8,
    HAND     = 9,
    CHEMS    = 10,
    STIMPACK = 11,
    FOOD     = 12,
    ALCOHOL  = 13,
};

};
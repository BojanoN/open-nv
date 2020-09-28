#pragma once
#include <cstdint>

namespace ESM {

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

};
#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

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

enum class EffectDataFlags : uint8_t {
    NO_AUTO_CALCULATION,
    FOOD
};

struct __attribute__((packed)) IngredientEffectData {
    int32_t         value;
    EffectDataFlags flag;
    uint8_t         unused[3];
};

struct Ingredient : public Record {
    std::string          editorId;
    ObjectBounds         objectBounds;
    std::string          name;
    ModelData            modelData;
    std::string          largeIconFilename;
    std::string          smallIconFilename;
    formid               script;
    EquipmentType        equipmentType;
    float                weight;
    IngredientEffectData effectData;
    std::vector<Effect>  effects;

    Ingredient(ESMReader& reader);
};

}

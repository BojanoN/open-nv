#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

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

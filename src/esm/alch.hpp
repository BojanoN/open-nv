#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/destructiondata.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/effect.hpp"
#include "subrecords/equipment_type.hpp"
#include "structs.hpp"

namespace ESM {
enum class AlchemyFlag : uint8_t {
    NoAutoCalc = 0x01,
    Food       = 0x02,
    Medicine   = 0x04
};

struct IngestibleEffectData {
    int32_t     value; // ??
    AlchemyFlag flags;
    uint8_t     unused[3];
    formid      withdrawalEffect; // SPEL
    float       addictionChance;
    formid      consumeSound; // SOUN
};

struct Ingestible : public Record {

    ObjectBounds         objectBounds;
    std::string          name;
    ModelData            modelData;
    std::string          largeIconFilename;
    std::string          smallIconFilename;
    formid               script;
    DestructionData      destructionData;
    formid               pickUpSound;
    formid               dropSound;
    EquipmentType        equipmentType;
    float                weight;
    IngestibleEffectData effectData;
    std::vector<Effect>  effects;

    Ingestible(ESMReader& reader);
};
}

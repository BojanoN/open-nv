#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/destructiondata.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

struct Furniture : public Record {
    ObjectBounds         objectBounds;
    std::string          name;
    ModelData            modelData;
    formid               script;
    DestructionData      destructionData;
    std::vector<uint8_t> markerFlags;

    Furniture(ESMReader& reader);
};
}

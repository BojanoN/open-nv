#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;
struct Furniture : public Record {
    std::string          editorId;
    ObjectBounds         objectBounds;
    std::string          name;
    ModelData            modelData;
    formid               script;
    DestructionData      destructionData;
    std::vector<uint8_t> markerFlags;

    Furniture(ESMReader& reader);
};
}

#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

#include <vector>

namespace ESM {

class ESMReader;

enum class DebrisModelDataFlags : uint8_t {
    HasCollisionData = 0x1
};

struct DebrisModelData {
    uint8_t              percentage;
    std::string          modelFilename;
    DebrisModelDataFlags flags;
};

struct DebrisModel {
    DebrisModelData      modelData;
    std::vector<uint8_t> textureFileHashes;
};

struct Debris : public Record {
    std::vector<DebrisModel> debrisModels;

    Debris(ESMReader& reader);
};

}

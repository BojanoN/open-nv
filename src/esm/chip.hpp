#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct CasinoChip : public Record {
    ObjectBounds    objectBounds;
    std::string     name;
    ModelData       modelData;
    std::string     largeIconFilename;
    std::string     smallIconFilename;
    DestructionData destructionData;
    formid          pickUpSound;
    formid          dropSound;

    CasinoChip(ESMReader& reader);
};
}
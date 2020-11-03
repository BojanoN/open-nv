#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/destructiondata.hpp"
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
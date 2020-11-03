#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/destructiondata.hpp"
#include "structs.hpp"

namespace ESM {

struct ItemMod : public Record {
    ObjectBounds    objectBounds;
    std::string     name;
    ModelData       modelData;
    std::string     largeIconFilename;
    std::string     smallIconFilename;
    formid          script;
    std::string     description;
    DestructionData destructionData;
    formid          pickUpSound;
    formid          dropSound;
    formid          loopingSound;
    ItemQuality     data;

    ItemMod(ESMReader& reader);
};
}
#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

struct Item : public Record {
    ObjectBounds    objectBounds;
    std::string     name;
    ModelData       modelData;
    std::string     largeIconFilename;
    std::string     smallIconFilename;
    formid          script;
    DestructionData destructionData;
    formid          pickUpSound;
    formid          dropSound;
    formid          loopingSound;
    ItemQuality     data;

    Item(ESMReader& reader);
};
}

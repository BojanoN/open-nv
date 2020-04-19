#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

struct __attribute__((packed)) MiscItemData {
    int32_t value;
    float   weight;
};

struct MiscItem : public Record {
    std::string     editorId;
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
    MiscItemData    data;

    MiscItem(ESMReader& reader);
};
}

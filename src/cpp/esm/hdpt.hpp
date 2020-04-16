#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct HeadPart : public Record {

    std::string         editorId;
    std::string         name;
    ModelData           modelData; //Always only 1 for headpart.
    uint8_t             flags;
    std::vector<formid> extraParts;

    HeadPart(ESMReader& reader);
};

}

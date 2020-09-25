#pragma once
#include "record.hpp"
#include "subrecords/modeldata.hpp"
#include "structs.hpp"

#include <unordered_set>

namespace ESM {

class ESMReader;

struct HeadPart : public Record {

    std::string         name;
    ModelData           modelData; //Always only 1 for headpart.
    uint8_t             flags;
    std::vector<formid> extraParts;

    HeadPart(ESMReader& reader);
};

}

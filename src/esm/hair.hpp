#pragma once

#include "record.hpp"
#include "structs.hpp"
#include <unordered_set>

namespace ESM {

class ESMReader;

struct Hair : public Record {

    std::string name;
    ModelData   modelData;
    std::string texture;
    uint8_t     flags;

    Hair(ESMReader& reader);
};

};

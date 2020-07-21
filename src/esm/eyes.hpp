#pragma once

#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct Eyes : public Record {

    std::string name;
    std::string texture;
    uint8_t     flags;

    Eyes(ESMReader& reader);
};

}

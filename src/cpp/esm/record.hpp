#pragma once
#include "reader.hpp"

namespace ESM {

struct Record {
    uint32_t type;
    uint32_t recordFlags;
    formid   id;

    Record() {};
    Record(ESMReader& reader)
        : type { reader.recordType() }
        , recordFlags { reader.recordFlags() }
        , id { reader.recordId() }
    {
    }
    ~Record() {};

    //virtual void load(ESMReader& reader) = 0;
};

};

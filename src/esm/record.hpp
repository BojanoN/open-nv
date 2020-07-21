#pragma once
#include "reader.hpp"

namespace ESM {

struct Record {
    uint32_t    recordType;
    RecordFlags recordFlags;
    formid      id;
    std::string editorId = "";

    Record() {};
    Record(ESMReader& reader)
        : recordType { reader.recordType() }
        , recordFlags { reader.recordFlags() }
        , id { reader.recordId() }

    {
    }
    virtual ~Record() {};
};

};

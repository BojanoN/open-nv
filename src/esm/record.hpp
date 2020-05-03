#pragma once
#include "reader.hpp"

namespace ESM {

struct Record {
    uint32_t    recordType;
    RecordFlags recordFlags;
    formid      id;

    Record() {};
    Record(ESMReader& reader)
        : recordType { reader.recordType() }
        , recordFlags { reader.recordFlags() }
        , id { reader.recordId() }
    {
        /*if (this->recordFlags & RecordFlags::COMPRESSED) {
            reader.readCompressed();
        }*/
    }
    virtual ~Record() {};
};

};

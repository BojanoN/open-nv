#pragma once
#include "reader.hpp"

namespace ESM {

struct Record {
    std::string editorId = "";
    uint32_t    recordType;
    RecordFlags recordFlags;
    formid      id;

    Record() {};
    Record(ESMReader& reader)
        : recordType { reader.recordType() }
        , recordFlags { reader.recordFlags() }
        , id { reader.recordId() }

    {
    }
    virtual ~Record() {};
};

class ScriptableRecord : public Record {
public:
    ScriptableRecord(ESMReader& reader)
        : Record(reader) {};

    virtual formid getLinkedScript() = 0;
};

};

#include "scpt.hpp"
#include "reader.hpp"

namespace ESM {

Script::Script(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    ScriptData::load(reader, this->data);
}

};

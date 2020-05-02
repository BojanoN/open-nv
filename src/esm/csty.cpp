#include "csty.hpp"
#include "reader.hpp"

namespace ESM {
CombatStyle::CombatStyle(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::CSTD);
    reader.readSubrecordTypeSize(this->standard);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::CSAD);
    reader.readSubrecordTypeSize(this->advanced);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::CSSD);
    reader.readSubrecordTypeSize(this->simple);
}
}

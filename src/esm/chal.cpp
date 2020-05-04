#include "chal.hpp"
#include "reader.hpp"

namespace ESM {

Challenge::Challenge(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->pipboyTexture);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->cornerTexture);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
            break;
        case ESMType::DESC:
            reader.readStringSubrecord(this->description);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->data);
            break;
        case ESMType::SNAM:
            reader.readSubrecord(this->value3);
            break;
        case ESMType::XNAM:
            reader.readSubrecord(this->value4);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}

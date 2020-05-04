#include "ccrd.hpp"
#include "reader.hpp"

namespace ESM {

CaravanCard::CaravanCard(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(this->objectBounds);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::YNAM:
            reader.readSubrecord(this->pickUpSound);
            break;
        case ESMType::ZNAM:
            reader.readSubrecord(this->dropSound);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
            break;
        case ESMType::INTV:
            reader.readSubrecord(this->cardSuit);
            reader.readNextSubrecordHeader();
            reader.checkSubrecordHeader(ESMType::INTV);
            reader.readSubrecord(this->cardValue);
            break;
        case ESMType::TX00:
            reader.readStringSubrecord(this->highResImageFace);
            break;
        case ESMType::TX01:
            reader.readStringSubrecord(this->highResImageBack);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->value);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}

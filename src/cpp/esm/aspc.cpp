#include "aspc.hpp"
#include "reader.hpp"

namespace ESM {

AcousticSpace::AcousticSpace(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(this->objectBounds);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.readSubrecord(this->dawn_default);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.readSubrecord(this->afternoon);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.readSubrecord(this->dusk);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.readSubrecord(this->night);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.readSubrecord(this->walla);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::WNAM);
    reader.readSubrecord(this->wallaTriggerCount);

    if (reader.peekNextType() == ESMType::RDAT) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(this->regionSound);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ANAM);
    reader.readSubrecord(this->environmentType);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::INAM);
    reader.readSubrecord(this->isInterior);
}

};

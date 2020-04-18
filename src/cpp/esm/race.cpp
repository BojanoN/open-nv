#include "race.hpp"
#include "reader.hpp"

namespace ESM {
Race::Race(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readStringSubrecord(this->name);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DESC);
    reader.readStringSubrecord(this->description);

    while (reader.peekNextType() == ESMType::XNAM) {
        reader.readNextSubrecordHeader();
        relations.emplace_back();
        reader.readSubrecord(relations.back());
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(this->raceData);

    if (reader.peekNextType() == ESMType::ONAM) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(this->older);
    }

    if (reader.peekNextType() == ESMType::YNAM) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(this->younger);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM2);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::VTCK);
    reader.readSubrecord(this->voices);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DNAM);
    reader.readSubrecord(this->defaultHairStyle);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::CNAM);
    reader.readSubrecord(this->defaultHairColor);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::PNAM);
    reader.readSubrecord(this->FaceGenMainClamp);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::UNAM);
    reader.readSubrecord(this->FaceGenFaceClamp);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ATTR);
    reader.readFixedArraySubrecord(this->unknown);

    uint32_t index = 0;
    while (reader.peekNextType() != ESMType::FNAM) {
        maleHeadParts.emplace_back();
        ModelData::load(reader, maleHeadParts.back(), index, )
    }
}
};

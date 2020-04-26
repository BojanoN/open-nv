#include "wthr.hpp"
#include "reader.hpp"

namespace ESM {

Weather::Weather(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.peekNextType() != ESMType::LNAM) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case ESMType::IAD0:
            reader.readSubrecord(sunriseImageModifier);
            break;
        case ESMType::IAD1:
            reader.readSubrecord(dayImageModifier);
            break;
        case ESMType::IAD2:
            reader.readSubrecord(sunsetImageModifier);
            break;
        case ESMType::IAD3:
            reader.readSubrecord(nightImageModifier);
            break;
        case ESMType::IAD4:
            reader.readSubrecord(noonImageModifier);
            break;
        case ESMType::IAD5:
            reader.readSubrecord(midnightImageModifier);
            break;
        case ESMType::DNAM:
            reader.readStringSubrecord(this->cloudTextL0);
            break;
        case ESMType::CNAM:
            reader.readStringSubrecord(this->cloudTextL1);
            break;
        case ESMType::ANAM:
            reader.readStringSubrecord(this->cloudTextL2);
            break;
        case ESMType::BNAM:
            reader.readStringSubrecord(this->cloudTextL3);
            break;

        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::LNAM);
    reader.skipSubrecord();

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ONAM);
    reader.readFixedArraySubrecord(this->cloudLayerSpeed);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::PNAM);
    reader.readFixedArraySubrecord(this->cloudLayerColor);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM0);
    // A couple of subrecords have 160 instead of 240
    // TODO: check the GECK for formid 601032
    reader.readRawDataSubrecSize(this->colorsType);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FNAM);
    reader.readSubrecord(this->fogDistance);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::INAM);
    reader.skipSubrecord();

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(this->data);

    while (reader.peekNextType() == ESMType::SNAM) {
        reader.readNextSubrecordHeader();
        this->weatherSounds.emplace_back();
        reader.readSubrecord(this->weatherSounds.back());
    }
}
}

#include "wrld.hpp"
#include "reader.hpp"

namespace ESM {

Worldspace::Worldspace(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    while (reader.peekNextType() != ESMType::DATA) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::XEZN:
            reader.readSubrecord(this->encounterZone);
            break;
        case ESMType::WNAM:
            reader.readSubrecord(this->parentWorldspace);
            break;
        case ESMType::PNAM:
            reader.readSubrecord(this->parentFlags);
            break;
        case ESMType::CNAM:
            reader.readSubrecord(this->climate);
            break;
        case ESMType::NAM2:
            reader.readSubrecord(this->water);
            break;
        case ESMType::NAM3:
            reader.readSubrecord(this->lodWater);
            break;
        case ESMType::NAM4:
            reader.readSubrecord(this->lodWaterHeight);
            break;
        case ESMType::DNAM:
            reader.readSubrecord(this->landData);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::MNAM:
            reader.readSubrecord(this->mapData);
            break;
        case ESMType::ONAM:
            reader.readSubrecord(this->mapOffsetData);
            break;
        case ESMType::INAM:
            reader.readSubrecord(this->imageSpace);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
    reader.readNextSubrecordHeader();
    reader.readSubrecord(this->flags);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM0);
    reader.readSubrecord(this->minObjectBounds);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM9);
    reader.readSubrecord(this->maxObjectBounds);

    if (reader.peekNextType() == ESMType::ZNAM) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(this->music);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NNAM);
    reader.readStringSubrecord(this->canopyShadow);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::XNAM);
    reader.readStringSubrecord(this->waterNoiseTexture);

    if (reader.peekNextType() == ESMType::IMPF) {
        reader.readNextSubrecordHeader();
        reader.readArraySubrecord(this->swappedImpact);
    }

    if (reader.peekNextType() == ESMType::IMPF) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(this->footstepMaterial);
    }

    if (reader.peekNextType() == ESMType::XXXX) {
        reader.readNextSubrecordHeader();
        reader.skipRecord();
        return;
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OFST);
    reader.readArraySubrecord(this->offsetData);
}
}

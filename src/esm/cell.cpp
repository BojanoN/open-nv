#include "cell.hpp"

namespace ESM {

Cell::Cell(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    if (reader.subrecordType() == ESMType::EDID) {
        reader.checkSubrecordHeader(ESMType::EDID);
        reader.readStringSubrecord(editorId);
        reader.readNextSubrecordHeader();
    }

    if (reader.subrecordType() == ESMType::FULL) {
        reader.readStringSubrecord(name);
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(flags);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::XCLC:
            reader.readSubrecord(grid);
            break;
        case ESMType::XCLL:
            reader.readSubrecord(lighting);
            break;
        case ESMType::IMPF:
            reader.readSubrecord(footstepMaterial);
            break;
        case ESMType::LTMP:
            reader.readSubrecord(lightTemplate.templ);
            break;
        case ESMType::LNAM:
            reader.readSubrecord(lightTemplate.inheritFlags);
            break;
        case ESMType::XCLW:
            reader.readSubrecord(waterHeight);
            break;
        case ESMType::XNAM:
            reader.readStringSubrecord(waterNoiseTexture);
            break;
        case ESMType::XCLR:
            reader.readArraySubrecord(regions);
            break;
        case ESMType::XCIM:
            reader.readSubrecord(imageSpace);
            break;
        case ESMType::XCET:
            reader.readSubrecord(unknown);
            break;
        case ESMType::XEZN:
            reader.readSubrecord(encounterZone);
            break;
        case ESMType::XCCM:
            reader.readSubrecord(climate);
            break;
        case ESMType::XCWT:
            reader.readSubrecord(water);
            break;
        case ESMType::XOWN:
            reader.readSubrecord(owner);
            break;
        case ESMType::XRNK:
            reader.readSubrecord(factionRank);
            break;
        case ESMType::XCAS:
            reader.readSubrecord(acousticSpace);
            break;
        case ESMType::XCMT:
            reader.readSubrecord(unused);
            break;
        case ESMType::XCMO:
            reader.readSubrecord(musicType);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}

};

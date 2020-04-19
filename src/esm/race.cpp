#include "race.hpp"
#include "headers.hpp"
#include "reader.hpp"

namespace ESM {

void ModelPart::load(ESMReader& reader, std::vector<ModelPart>& parts, ESMType nextType)
{
    do {
        reader.readNextSubrecordHeader();
        parts.emplace_back();
        std::unordered_set<ESMType> nexts{ESMType::ICON, ESMType::MICO, ESMType::INDX, nextType};

        switch (reader.subrecordType()) {
        case (ESMType::INDX):
            reader.readSubrecord(parts.back().index);
            break;
        case (ESMType::MODL):
            ModelData::load(reader, parts.back().modelData, 0, nexts);
            break;
        case (ESMType::ICON):
            reader.readStringSubrecord(parts.back().largeIconFilename);
            break;
        case (ESMType::MICO):
            reader.readStringSubrecord(parts.back().smallIconFilename);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            log_fatal(s.str().c_str());
            reader.reportError(s.str());
            break;
        } 

    } while(reader.peekNextType() != nextType);
}

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

    if (reader.peekNextType() == ESMType::DNAM) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::DNAM);
        reader.readSubrecord(this->defaultHairStyle);
    }

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

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM0);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MNAM);
    ModelPart::load(reader, this->maleHeadParts, ESMType::FNAM);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FNAM);
    ModelPart::load(reader, this->femaleHeadParts, ESMType::NAM1);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM1);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MNAM);
    ModelPart::load(reader, this->maleBodyParts, ESMType::FNAM);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FNAM);
    ModelPart::load(reader, this->femaleHeadParts, ESMType::HNAM);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::HNAM);
    reader.readArraySubrecord(this->hairs);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ENAM);
    reader.readArraySubrecord(this->eyes);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MNAM);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FGGS);
    reader.readArraySubrecord(this->maleFaceGenGeometry_Symmetric);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FGGA);
    reader.readArraySubrecord(this->maleFaceGenGeometry_Asymmetric);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FGTS);
    reader.readArraySubrecord(this->maleFaceGenTexture_Symmetric);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.skipSubrecord();

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FNAM);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FGGS);
    reader.readArraySubrecord(this->femaleFaceGenGeometry_Symmetric);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FGGA);
    reader.readArraySubrecord(this->femaleFaceGenGeometry_Asymmetric);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FGTS);
    reader.readArraySubrecord(this->femaleFaceGenTexture_Symmetric);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.skipSubrecord();
}
};

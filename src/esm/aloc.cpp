#include "aloc.hpp"
#include "reader.hpp"

namespace ESM {

MediaLocationController::MediaLocationController(ESMReader& reader)
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
        case ESMType::NAM4:
            reader.readSubrecord(this->locationDelay);
            break;
        case ESMType::NAM5:
            reader.readSubrecord(this->dayStart);
            break;
        case ESMType::NAM6:
            reader.readSubrecord(this->nightStart);
            break;
        case ESMType::NAM7:
            reader.readSubrecord(this->retriggerDelay);
            break;
        case ESMType::HNAM:
            reader.readArraySubrecord(this->neutralMediaSet);
            break;
        case ESMType::ZNAM:
            reader.readArraySubrecord(this->allyMediaSet);
            break;
        case ESMType::XNAM:
            reader.readArraySubrecord(this->friendMediaSet);
            break;
        case ESMType::YNAM:
            reader.readArraySubrecord(this->enemyMediaSet);
            break;
        case ESMType::LNAM:
            reader.readArraySubrecord(this->locationMediaSet);
            break;
        case ESMType::GNAM:
            reader.readArraySubrecord(this->battleMediaSet);
            break;
        case ESMType::RNAM:
            reader.readSubrecord(this->conditionalFaction);
            break;
        case ESMType::NAM1:
        case ESMType::NAM2:
        case ESMType::NAM3:
        case ESMType::FNAM:
            reader.skipSubrecord();
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}

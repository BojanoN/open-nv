#include "mset.hpp"
#include "reader.hpp"

namespace ESM {

MediaSet::MediaSet(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    uint32_t jnamCount = 0;

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::NAM1:
            reader.readSubrecord(this->type);
            break;
        case ESMType::NAM2:
            reader.readStringSubrecord(this->loopBattleDayOuter);
            break;
        case ESMType::NAM3:
            reader.readStringSubrecord(this->exploreDayMiddle);
            break;
        case ESMType::NAM4:
            reader.readStringSubrecord(this->suspenseDayInner);
            break;
        case ESMType::NAM5:
            reader.readStringSubrecord(this->nightOuter);
            break;
        case ESMType::NAM6:
            reader.readStringSubrecord(this->nightMiddle);
            break;
        case ESMType::NAM7:
            reader.readStringSubrecord(this->nightInner);
            break;
        case ESMType::NAM8:
            reader.readSubrecord(this->loopBattleDayOuterdB);
            break;
        case ESMType::NAM9:
            reader.readSubrecord(this->exploreDayMiddledB);
            break;
        case ESMType::NAM0:
            reader.readSubrecord(this->suspenseDayInnerdB);
            break;
        case ESMType::ANAM:
            reader.readSubrecord(this->nightOuterdB);
            break;
        case ESMType::BNAM:
            reader.readSubrecord(this->nightMiddledB);
            break;
        case ESMType::CNAM:
            reader.readSubrecord(this->nightInnerdB);
            break;
        case ESMType::JNAM: {
            switch (jnamCount) {
            case 0:
                reader.readSubrecord(this->dayOuterBoundPercent);
                break;
            case 1:
                reader.readSubrecord(this->dayMiddleBoundPercent);
                break;
            case 2:
                reader.readSubrecord(this->dayInnerBoundPercent);
                break;
            case 3:
                reader.readSubrecord(this->nightOuterBoundPercent);
                break;
            case 4:
                reader.readSubrecord(this->nightMiddleBoundPercent);
                break;
            case 5:
                reader.readSubrecord(this->nightInnerBoundPercent);
                break;
            default:
                reader.reportError("Too many JNAM entries while parsing MSET");
            }
            jnamCount++;
            break;
        }
        case ESMType::PNAM:
            reader.readSubrecord(this->enableFlags);
            break;
        case ESMType::DNAM:
            reader.readSubrecord(this->waitTime);
            break;
        case ESMType::ENAM:
            reader.readSubrecord(this->loopFadeOut);
            break;
        case ESMType::FNAM:
            reader.readSubrecord(this->recoveryTime);
            break;
        case ESMType::GNAM:
            reader.readSubrecord(this->nightTimeMax);
            break;
        case ESMType::HNAM:
            reader.readSubrecord(this->intro);
            break;
        case ESMType::INAM:
            reader.readSubrecord(this->outro);
            break;
        // TODO: further GECK investigation for these subrecords
        case ESMType::LNAM:
        case ESMType::NNAM:
        case ESMType::MNAM:
        case ESMType::ONAM:
        case ESMType::KNAM:
            reader.skipSubrecord();
            break;
        case ESMType::DATA:
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

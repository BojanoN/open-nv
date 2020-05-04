#include "pgre.hpp"
#include "reader.hpp"

namespace ESM {

PlacedGrenade::PlacedGrenade(ESMReader& reader)
    : Record(reader)
{

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case ESMType::EDID:
            reader.readStringSubrecord(editorId);
            break;
        case ESMType::NAME:
            reader.readSubrecord(base);
            break;
        case ESMType::XEZN:
            reader.readSubrecord(encounterZone);
            break;
        case ESMType::XRGD:
        case ESMType::XRGB:
            reader.skipSubrecord(); // ;_;
            break;
        case ESMType::XPRD:
            reader.readSubrecord(patrolData.idleTime);
            break;
        case ESMType::XPPA:
            reader.readNextSubrecordHeader();
            reader.checkSubrecordHeader(ESMType::INAM);
            reader.readSubrecord(patrolData.idleAnimation);
            ScriptData::load(reader, patrolData.patrolScript);
            reader.readNextSubrecordHeader();
            reader.checkSubrecordHeader(ESMType::TNAM);
            reader.readSubrecord(patrolData.dialogue);
            break;
        case ESMType::XOWN:
            reader.readSubrecord(ownershipData.owner);
            break;
        case ESMType::XRNK:
            reader.readSubrecord(ownershipData.factionRank);
            break;
        case ESMType::XCNT:
            reader.readSubrecord(count);
            break;
        case ESMType::XRDS:
            reader.readSubrecord(radius);
            break;
        case ESMType::XHLP:
            reader.readSubrecord(health);
            break;
        case ESMType::XPWR:
            this->waterReflection.emplace_back();
            reader.readSubrecord(this->waterReflection.back());
            break;
        case ESMType::XDCR:
            linkedDecals.emplace_back();
            reader.readSubrecord(linkedDecals.back());
            break;
        case ESMType::XLKR:
            reader.readSubrecord(linkedReference);
            break;
        case ESMType::XCLP:
            reader.readSubrecord(linkedReferenceColor);
            break;
        case ESMType::XAPD:
            reader.readSubrecord(activateParentsFlags);
            break;
        case ESMType::XAPR:
            activateParents.emplace_back();
            reader.readSubrecord(activateParents.back());
            break;
        case ESMType::XATO:
            reader.readStringSubrecord(activationPrompt);
            break;
        case ESMType::XESP:
            reader.readSubrecord(enableParent);
            break;
        case ESMType::XEMI:
            reader.readSubrecord(emittance);
            break;
        case ESMType::XMBR:
            reader.readSubrecord(multiBoundReference);
            break;
        case ESMType::XIBS:
            ignoredBySandBox = true;
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->positionRotation);
            break;
        case ESMType::XSCL:
            reader.readSubrecord(scale);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};

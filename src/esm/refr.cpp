#include "refr.hpp"

namespace ESM {

PlacedObject::PlacedObject(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    bool rclrFlag = false;

    while (reader.peekNextType() != ESMType::DATA) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case ESMType::NAME:
            reader.readSubrecord(base);
            break;
        case ESMType::XEZN: {
            reader.readSubrecord(encounterZone);

        case ESMType::RCLR:
            if (!rclrFlag) {
                reader.readSubrecord(this->linkedReferenceColor);
                rclrFlag = true;
            } else {
                reader.skipSubrecord();
            }
            break;
        }
        case ESMType::FULL:
        case ESMType::BNAM:
        case ESMType::MNAM:
        case ESMType::NNAM:
        case ESMType::XSRF:
        case ESMType::XMBP:
        case ESMType::XSRD:
        case ESMType::ONAM:
        case ESMType::XLOD:
        case ESMType::XRGD:
        case ESMType::XRGB:
            reader.skipSubrecord(); // ;_;
            break;
        case ESMType::XPRD:
            reader.readSubrecord(patrolData.idleTime);
            break;
        case ESMType::XPRM:
            reader.readSubrecord(this->primitive);
            break;
        case ESMType::XTRI:
            reader.readSubrecord(this->collisionLayerValues);
            break;
        case ESMType::XMBO:
            reader.readSubrecord(this->halfExtents);
            break;
        case ESMType::XTEL:
            reader.readSubrecord(this->teleportDest);
            break;
        case ESMType::FNAM:
            reader.readSubrecord(this->mapMarkerFlags);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(this->mapMarkerName);
            break;
        case ESMType::TNAM:
            reader.readSubrecord(this->mapMarkerData);
            break;
        case ESMType::WMI1:
            reader.readSubrecord(this->mapMarkerReputation);
            break;
        case ESMType::MMRK: {
            reader.skipSubrecord();
            if (reader.peekNextType() == ESMType::FULL) {
                reader.readNextSubrecordHeader();
                reader.skipSubrecord;
            }
            break;
        }
        case ESMType::CNAM:
            reader.readSubrecord(this->audioLocation);
            break;
        case ESMType::XTRG:
            reader.readSubrecord(this->target);
            break;
        case ESMType::XRDO:
            reader.readSubrecord(this->radioData);
            break;

        case ESMType::XPPA:
            reader.readNextSubrecordHeader();
            reader.checkSubrecordHeader(ESMType::INAM);
            reader.readSubrecord(patrolData.idleAnimation);
            ScriptData::load(reader, patrolData.patrolScript);
            break;
        case ESMType::TNAM:
            reader.readSubrecord(patrolData.dialogue);
            break;
        case ESMType::XLCM:
            reader.readSubrecord(levelModifier);
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
        case ESMType::XRAD:
            reader.readSubrecord(radiation);
            break;
        case ESMType::XCHG:
            reader.readSubrecord(charge);
            break;
        case ESMType::XAMT:
            reader.readSubrecord(ammoType);
            break;
        case ESMType::XAMC:
            reader.readSubrecord(ammoCount);
            break;
        case ESMType::XPWR:
            this->waterReflection.emplace_back();
            reader.readSubrecord(this->waterReflection.back());
            break;
        case ESMType::XLTW:
            reader.readSubrecord(litWater);
            break;
        case ESMType::XDCR:
            linkedDecals.emplace_back();
            reader.readSubrecord(linkedDecals.back());
            break;
        case ESMType::XLKR:
            reader.readSubrecord(linkedReference);
            break;
        case ESMType::XCLP:
            reader.readSubrecord(altReferenceColor);
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
            reader.readSubrecord(emmitance);
            break;
        case ESMType::XMBR:
            reader.readSubrecord(multiBoundReference);
            break;
        case ESMType::XIBS:
            ignoredBySandBox = true;
            break;
        case ESMType::XNDP:
            reader.readSubrecord(navDoorLink);
            break;
        case ESMType::XPOD:
            reader.readArraySubrecord(portalRooms);
            break;
        case ESMType::XPTL:
            reader.readSubrecord(portalData);
            break;
        case ESMType::XSED:
            reader.readSubrecord(speedTreeSeed);
            break;
        case ESMType::XRMR:
            reader.readSubrecord(roomHeader);
            break;
        case ESMType::XLRM:
            reader.readSubrecord(linkedRoom);
            break;
        case ESMType::XOCP:
            reader.readSubrecord(occlusionPlane);
            break;
        case ESMType::XORD:
            reader.readSubrecord(linkedOcclusionPlanes);
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
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(positionRotation);
}

};

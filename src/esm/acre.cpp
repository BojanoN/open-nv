#include "acre.hpp"

namespace ESM {

PlacedCreature::PlacedCreature(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAME);
    reader.readSubrecord(base);

    while(reader.peekNextType() != ESMType::XPRD) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::XEZN:
    			reader.readSubrecord(encounterZone);
    			break;
    		case ESMType::XRGD:
    		case ESMType::XRGB:
    			reader.skipSubrecord(); // ;_;
    			break;
    		default:
    			std::stringstream s;
            	s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            	reader.reportError(s.str());
    	}
    }
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::XPRD);
    reader.readSubrecord(patrolData.idleTime);
    
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::XPPA);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::INAM);
    reader.readSubrecord(patrolData.idleAnimation);
    ScriptData::load(reader, patrolData.patrolScript);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DIAL);
    reader.readSubrecord(patrolData.dialogue);

    while(reader.peekNextType() != ESMType::DATA) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::XLCM:
    			reader.readSubrecord(levelModifier);
    			break;
    		case ESMType::XOWN:
    			reader.readSubrecord(ownershipData.owner);
    			break;
    		case ESMType::XRNK:
    			reader.readSubrecord(ownershipData.factionRank);
    			break;
    		case ESMType::XMRC:
    			reader.readSubrecord(merchantContainer);
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
    			reader.readSubrecord(emmitance);
    			break;
    		case ESMType::XMBR:
    			reader.readSubrecord(multiBoundReference);
    			break;
    		case ESMType::XIBS:
    			ignoredBySandBox = true;
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
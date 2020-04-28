#include "dial.hpp"

namespace ESM {

DialogueTopic::DialogueTopic(ESMReader& reader) : Record(reader) {

	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while(reader.peekNextType() != ESMType::FULL) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::QSTI:
    			addedQuests.emplace_back();
    			AddedQuest::load(addedQuests.back(), reader);
    			break;
    		case ESMType::QSTR:
    			removedQuests.emplace_back();
    			reader.readSubrecord(removedQuests.back());
    			break;
    		case ESMType::INFC: //unused subrecords due to GECK bug
    		case ESMType::INFX:
    			reader.skipSubrecord();
    			break;
    		default:
    			std::stringstream s;
            	s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            	reader.reportError(s.str());
    	}
    }
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readStringSubrecord(name);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::PNAM);
    reader.readSubrecord(priority);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::TDUM) {
    	reader.readStringSubrecord(tdum);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);
}


void AddedQuest::load(AddedQuest& addedQuest, ESMReader& reader) {
	reader.checkSubrecordHeader(ESMType::QSTI);
	reader.readSubrecord(addedQuest.quest);

	while(reader.peekNextType() == ESMType::INFC) {
		addedQuest.sharedInfos.emplace_back();
		reader.readNextSubrecordHeader();
		reader.readSubrecord(addedQuest.sharedInfos.back().infoConnection);
		reader.readNextSubrecordHeader();
		reader.checkSubrecordHeader(ESMType::INFX);
		reader.readSubrecord(addedQuest.sharedInfos.back().infoIndex);
	}
}

}
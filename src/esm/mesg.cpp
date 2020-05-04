#include "mesg.hpp"


namespace ESM {

void MenuButton::load(ESMReader& reader, MenuButton& button) {
	reader.readStringSubrecord(button.buttonText);
	while(reader.peekNextType() == ESMType::CTDA) {
		reader.readNextSubrecordHeader();
		button.conditions.emplace_back();
		Condition::load(reader, button.conditions.back());
	}
}

Message::Message(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DESC);
    reader.readStringSubrecord(description);
    
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::FULL) {
    	reader.readStringSubrecord(name);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::INAM);
    reader.readSubrecord(icon);

    while(reader.hasMoreSubrecords()) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::NAM0:
    		case ESMType::NAM1:
    		case ESMType::NAM2:
    		case ESMType::NAM3:
    		case ESMType::NAM4:
    		case ESMType::NAM5:
    		case ESMType::NAM6:
    		case ESMType::NAM7:
    		case ESMType::NAM8:
    		case ESMType::NAM9:
    			reader.skipSubrecord();
    			break;
    		case ESMType::DNAM:
    			reader.readSubrecord(flags);
    			break;
    		case ESMType::TNAM:
    			reader.readSubrecord(displayTime);
    			break;
    		case ESMType::ITXT:
    			menuButtons.emplace_back();
    			MenuButton::load(reader, menuButtons.back());
    			break;    
    		default:
    			std::stringstream s;
    			s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
    			reader.reportError(s.str()); 
    	}
    }

}

};
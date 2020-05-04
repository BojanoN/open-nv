#include "avif.hpp"


namespace ESM {

ActorValueInformation::ActorValueInformation(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::FULL) {
    	reader.readStringSubrecord(name);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::DESC);
    reader.readStringSubrecord(description);
    while(reader.hasMoreSubrecords()) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::ICON:
    			reader.readStringSubrecord(largeIconFilename);
    			break;
    		case ESMType::MICO:
    			reader.readStringSubrecord(smallIconFilename);
    			break;
    		case ESMType::ANAM:
    		   	reader.readStringSubrecord(shortName);
    			break;
    		default:
    			std::stringstream s;
            	s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            	reader.reportError(s.str());
    	}
    }
}

};
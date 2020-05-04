#include "ipct.hpp"


namespace ESM {

Impact::Impact(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::MODL) {
        ModelData::load(reader, modelData, 0);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);

    while(reader.hasMoreSubrecords()) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::DODT:
    			reader.readSubrecord(decalData);
    			break;
    		case ESMType::DNAM:
    			reader.readSubrecord(textureSet);
    			break;
    		case ESMType::SNAM:
    			reader.readSubrecord(sound_1);
    			break;
    		case ESMType::NAM1:
    			reader.readSubrecord(sound_2);
    			break;
			default:
            	std::stringstream s;
            	s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            	reader.reportError(s.str());
    	}
    }
}

};
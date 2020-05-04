#include "musc.hpp"


namespace ESM {

MusicType::MusicType(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
	while (reader.hasMoreSubrecords()) {
    	reader.readNextSubrecordHeader();
    	switch (reader.subrecordType()) {
    	  case ESMType::FNAM:
    	    reader.readStringSubrecord(filename);
    	    break;
    	  case ESMType::ANAM:
    	    reader.readSubrecord(db);
    	    break;
    	  default:
    	    std::stringstream s;
    	    s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
    	    throw std::runtime_error(s.str());
    	}
  }
}

};
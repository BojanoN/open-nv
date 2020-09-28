#include "tes4.hpp"

namespace ESM {

MasterPluginInfo::MasterPluginInfo(ESMReader& reader) : Record(reader) {

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::HEDR);
	reader.readSubrecord(header);

	reader.readNextSubrecordHeader();
	if(reader.subrecordType() == ESMType::OFST) {
		reader.skipSubrecord();
	}
	if(reader.subrecordType() == ESMType::DELE) {
		reader.skipSubrecord();
	}

	reader.checkSubrecordHeader(ESMType::CNAM);
	reader.readStringSubrecord(author);

	while(reader.hasMoreSubrecords()) {
		
		reader.readNextSubrecordHeader();
		
		switch(reader.subrecordType()) {

			case ESMType::SNAM:
				reader.readStringSubrecord(description);
				break;
			case ESMType::MAST:
				parentMasters.emplace_back();
				reader.readStringSubrecord(parentMasters.back());
				break;
			case ESMType::DATA:
				reader.skipSubrecord();
				break;
			case ESMType::ONAM:
				reader.skipSubrecord();
				break;
			case ESMType::SCRN:
				reader.skipSubrecord();
				break;
			default:
				std::stringstream s;
        		s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
        		throw std::runtime_error(s.str());
		}
	}

}

};
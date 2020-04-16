#include "fact.hpp"

namespace ESM {

void Faction::loadRankCollection(ESMReader& reader) {
	bool first = true;
	while(reader.hasMoreSubrecords() && reader.subrecordType() != ESMType::WMI1) {
		if(first) {
			first = false;
		} else {
			reader.readNextSubrecordHeader();
		}
		
		switch(reader.subrecordType()) {
			case ESMType::RNAM:
				ranks.emplace_back();
				reader.readSubrecord(ranks.back().rankNumber);
				break;
			case ESMType::MNAM:
				reader.readStringSubrecord(ranks.back().male);
				break;
			case ESMType::FNAM:
				reader.readStringSubrecord(ranks.back().female);
				break;
			case ESMType::INAM:
				reader.readStringSubrecord(ranks.back().insignia);
				break;
		}
	}
}


Faction::Faction(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::EDID);
	reader.readStringSubrecord(editorId);

	while(reader.hasMoreSubrecords()) {
		reader.readNextSubrecordHeader();
		switch(reader.subrecordType()) {

			case ESMType::FULL:
				reader.readStringSubrecord(name);
				break;
			case ESMType::XNAM:
				relations.emplace_back();
				reader.readSubrecord(relations.back());
				break;
			case ESMType::DATA:
				reader.readSubrecord(factionData);
				break;
			case ESMType::CNAM:
				reader.readSubrecord(unused);
				break;
			case ESMType::RNAM:
				loadRankCollection(reader);
				break;
			case ESMType::WMI1:
				reader.readSubrecord(reputation);
				break;
			default:
				std::stringstream s;
				s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType());
				reader.reportError(s.str());
		}
	}


}

};
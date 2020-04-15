#include "fact.hpp"

namespace ESM {

struct FactionRank {
	int32_t rankNumber;
	std::string male;
	std::string female;
	std::string insignia;
};

void Faction::loadRankCollection(ESMReader& reader) {
	while(reader.hasMoreSubrecords() && reader.subrecordType().intValue != ESMType.WMI1) {
		switch(reader.subrecordType().intValue) {
			case ESMType.RNAM:
				ranks.push_back(new FactionRank);
				reader.readSubrecord<int32_t>(ranks.back().rankNumber);
				break;
			case ESMType.MNAM:
				reader.readArraySubrecord<char>(ranks.back().male.c_str());
				break;
			case ESMType.FNAM:
				reader.readArraySubrecord<char>(ranks.back().female.c_str());
				break;
			case ESMType.INAM:
				reader.readArraySubrecord<char>(ranks.back().insignia.c_str());
				break;
		}
		reader.readNextSubrecordHeader();
	}
}


void Faction::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType.EDID);
	reader.readArraySubrecord<char>(editorId.c_str());

	while(reader.hasMoreSubrecords()) {
		reader.readNextSubrecordHeader();
		switch(reader.subrecordType().intValue) {

			case ESMType.FULL:
				reader.readArraySubrecord<char>(name.c_str());
				break;
			case ESMType.XNAM:
				relations.push_back(new FactRaceRelation);
				reader.readSubrecord<FactRaceRelation>(relations.back());
				break;
			case ESMType.DATA:
				reader.readSubrecord<FactionData>(factionData);
				break;
			case ESMType.CNAM:
				reader.readSubrecord<float>(unused);
				break;
			case ESMType.RNAM:
				loadRankCollection(reader);
				break;
			case ESMType.WM1:
				reader.readSubrecord<formid>(reputation);
				break;
			default:
				throw std::runtime_error;
		}
	}


}

};
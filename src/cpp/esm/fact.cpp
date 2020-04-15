/*#include "fact.hpp"

namespace ESM {

struct FactionRank {
	int32_t rankNumber;
	std::string male;
	std::string female;
	std::string insignia;
};

void Faction::loadRankCollection(ESMReader& reader) {
	while(reader.hasMoreSubrecords() && reader.subrecordType().intValue != ESM::Names.WMI1) {
		switch(reader.subrecordType().intValue) {
			case ESM::Names.RNAM:
				ranks.push_back(new FactionRank);
				reader.readSubrecord<int32_t>(ranks.back().rankNumber);
				break;
			case ESM::Names.MNAM:
				reader.readArraySubrecord<char>(ranks.back().male.c_str());
				break;
			case ESM::Names.FNAM:
				reader.readArraySubrecord<char>(ranks.back().female.c_str());
				break;
			case ESM::Names.INAM:
				reader.readArraySubrecord<char>(ranks.back().insignia.c_str());
				break;
		}
		reader.readNextSubrecordHeader();
	}
}


void Faction::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.EDID);
	reader.readArraySubrecord<char>(editorId.c_str());

	while(reader.hasMoreSubrecords()) {
		reader.readNextSubrecordHeader();
		switch(reader.subrecordType().intValue) {

			case ESM::Names.FULL:
				reader.readArraySubrecord<char>(name.c_str());
				break;
			case ESM::Names.XNAM:
				relations.push_back(new FactRaceRelation);
				reader.readSubrecord<FactRaceRelation>(relations.back());
				break;
			case ESM::Names.DATA:
				reader.readSubrecord<FactionData>(factionData);
				break;
			case ESM::Names.CNAM:
				reader.readSubrecord<float>(unused);
				break;
			case ESM::Names.RNAM:
				loadRankCollection(reader);
				break;
			case ESM::Names.WM1:
				reader.readSubrecord<formid>(reputation);
				break;
			default:
				throw std::runtime_error;
		}
	}


}

};*/
/*#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct FactionData {
	uint8_t flags1;
	uint8_t flags2;
	uint8_t unused[2];
};

struct FactionRank {
	int32_t rankNumber;
	std::string male;
	std::string female;
	std::string insignia;
};


struct Faction : public Record {

	std::string editorId;
	std::string name;
	std::vector<FactRaceRelation> relations;
	FactionData factionData;
	float32 unused; //?
	std::vector<FactionRank> ranks;
	formid reputation;

	virtual void load(ESMReader& reader);
};


};*/
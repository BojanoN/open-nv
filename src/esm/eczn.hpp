#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


enum class EncounterZoneFlags : uint8_t {
	NEVER_RESETS = 0x01,
	MATCH_PC_BELOW_MINIMUM_LEVEL = 0x02
};
 
struct EncounterZoneData {
	formid owner; //NPC_ or FACT
	int8_t rank;
	int8_t minimumLevel;
	EncounterZoneFlags flags;
	uint8_t padding;
};

struct EncounterZone : public Record {

    std::string  editorId;
    EncounterZoneData data;

    EncounterZone(ESMReader& reader);
};
};
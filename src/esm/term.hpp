#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/destructiondata.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>

namespace ESM {

//#pragma pack(1)
struct TerminalData {
	struct TerminalDataPacked {
		uint8_t baseHackingDifficulty;
		uint8_t flags;
		uint8_t serverType;
	};

	uint8_t baseHackingDifficulty;
	uint8_t flags;
	uint8_t serverType;
	uint8_t unused;
};
//#pragma options align=reset


struct TerminalMenuItem {
	std::string itemText;
	std::string resultText;
	uint8_t flags;
	formid displayNote;
	formid subMenu;
	//Script script;
	std::vector<Condition> conditions;
};

struct Terminal : public Record {

	ObjectBounds objectBounds;
	std::string name;
	ModelData modelData;
	formid script;
	DestructionData destructionData;
	std::string description;
	formid soundLooping;
	formid passwordNote;
	TerminalData data;
	std::vector<TerminalMenuItem> menuItems;

	Terminal(ESMReader& reader);
};

};
#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct ArmorAddon : public Record {

	std::string editorId;
	ObjectBounds objectBounds;
	std::string name;
	BipedalData bipedData;
	
	ModelData maleBipedModelData;
	ModelData maleWorldModelData;
	std::string maleInventoryIconFilename;
	std::string maleMessageIconFilename;

	ModelData femaleBipedModelData;
	ModelData femaleWorldModelData;
	std::string femaleInventoryIconFilename;
	std::string femaleMessageIconFilename;

	int32_t equipmentType;
	ArmorData data;
	ArmorAttributes attributes;

	ArmorAddon(ESMReader& reader);
};

};
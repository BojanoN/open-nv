#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/armor_common.hpp"
#include "structs.hpp"

namespace ESM {

struct ArmorAddon : public Record {

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
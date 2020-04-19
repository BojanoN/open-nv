#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>
#include <sstream>

namespace ESM {

struct AnimationSound {
	formid sound;
	uint8_t chance;
	uint8_t unused[3];
	uint32_t type;
};

struct Armor : public Record {

	std::string editorId;
	ObjectBounds objectBounds;
	std::string name;
	formid script;
	formid objectEffect;
	BipedalData bipedData;
	
	ModelData maleBipedModelData;
	ModelData maleWorldModelData;
	std::string maleInventoryIconFilename;
	std::string maleMessageIconFilename;

	ModelData femaleBipedModelData;
	ModelData femaleWorldModelData;
	std::string femaleInventoryIconFilename;
	std::string femaleMessageIconFilename;

	std::string ragdollConstraintTemplate;
	formid repairList;
	formid bipedModelList;
	int32_t equipmentType;
	formid soundPickUp;
	formid soundDrop;

	ArmorData data;
	ArmorAttributes attributes;
	uint32_t overridesAnimationSounds;
	std::vector<AnimationSound> animationSounds;
	formid animationSoundsTemplate;

	Armor(ESMReader& reader);
};

};
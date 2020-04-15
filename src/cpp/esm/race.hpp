/*#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct SkillBoost {
	int8_t skill;
	int8_t boost;
};

struct RaceData {
	SkillBoost skillBoosts[7];
	uint8_t unused[2];
	float maleHeight;
	float femaleHeight;
	float maleWeight;
	float femaleWeight;
	uint32_t flags;
};

struct RaceVoice {
	formid male;
	formid female;
};

struct RaceDefaultHairStyle {
	formid male;
	formid female;
}

struct RaceDefaultHairColor {
	formid male;
	formid female;
}

struct ModelPart {
	uint32_t index;
	ModelData modelData;
	std::string largeIconFilename;
	std::string smallIconFilename;
};


struct Race : public Record {

	std::string editorId;
	std::string name;
	std::string description;
	std::vector<FactRaceRelation> relations;
	RaceData raceData;

	formid older;
	formid younger;

	RaceVoice voices;
	RaceDefaultHairStyle defaultHairStyle;
	RaceDefaultHairColor defaultHairColor;

	float FaceGenMainClamp;
	float FaceGenFaceClamp;

	uint8_t unknown[2];

	std::vector<ModelPart> maleHeadParts;
	std::vector<ModelPart> femaleHeadParts;
	std::vector<ModelPart> maleBodyParts;
	std::vector<ModelPart> femaleBodyParts;

	std::vector<formid> hairs;
	std::vector<formid> eyes;

	std::vector<uint8_t> maleFaceGenGeometry_Symmetric;
	std::vector<uint8_t> maleFaceGenGeometry_Asymmetric;
	std::vector<uint8_t> maleFaceGenTexture_Symmetric;

	std::vector<uint8_t> femaleFaceGenGeometry_Symmetric;
	std::vector<uint8_t> femaleFaceGenGeometry_Asymmetric;
	std::vector<uint8_t> femaleFaceGenTexture_Symmetric;

	virtual void load(ESMReader& reader);
};


};*/
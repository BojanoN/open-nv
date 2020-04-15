#include "reader.hpp"
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct ClassData {
	int32_t tagSkill1;
	int32_t tagSkill2;
	int32_t tagSkill3;
	int32_t tagSkill4;
	uint32_t flags;
	uint32_t services;
	int8_t teaches;
	uint8_t maximumTrainingLevel;
	uint8_t unused[2];
};

struct Class : public Record {

	std::string editorId;
	std::string name;
	std::string description;
	std::string largeIconFilename;
	std::string smallIconFilename;
	ClassData classData;
	Attributes attributes;

	virtual void load(ESMReader& reader);
};


};
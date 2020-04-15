#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


struct HeadPart : public Record {

	std::string editorId;
	std::string name;
	ModelData modelData; //Always only 1 for headpart.
	uint8_t flags;
	std::vector<formid> extraParts;

	virtual void load(ESMReader& reader);
};


};
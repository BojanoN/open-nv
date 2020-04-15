/*#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


struct Hair : public Record {

	std::string editorId;
	std::string name;
	ModelData modelData;
	std::string texture;
	uint8_t flags;

	virtual void load(ESMReader& reader);
};


};*/
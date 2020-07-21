#pragma once
#include "record.hpp"
#include "reader.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>
#include <sstream>

namespace ESM {

struct MoveableStatic : public Record {

	ObjectBounds objectBounds;
	std::string name;
	ModelData modelData;
	DestructionData destructionData;
	uint8_t unknown;
	formid sound;

	MoveableStatic(ESMReader& reader);

};

};
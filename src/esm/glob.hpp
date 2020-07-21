#pragma once
#include "reader.hpp"
#include "record.hpp"
#include <variant>
#include <sstream>

namespace ESM {

struct GlobalVariable : public Record {

	uint8_t type;
	std::variant<int32_t, int16_t, float> value;

	//All short variables are 4 bytes in the file for some reason

	GlobalVariable(ESMReader& reader);
};


};
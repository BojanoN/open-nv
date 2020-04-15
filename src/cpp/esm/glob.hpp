/*#pragma once
#include "reader.hpp"
#include "record.hpp"
#include <variant>

namespace ESM {

struct GlobalVariable : public Record {

	std::string editorId;
	uint8_t type;
	std::variant<int16_t, int32_t, float> value;

	virtual void load(ESMReader& reader);
};


};*/
#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>
#include <sstream>

namespace ESM {

enum LightFlags : uint32_t {
    DYNAMIC  		 = 0x1,
    CAN_BE_CARRIED   = 0x2,
    NEGATIVE      	 = 0x4,
    FLICKER          = 0x8,
    UNUSED           = 0x10,
    OFF_BY_DEFAULT   = 0x20,
    FLICKER_SLOW     = 0x40,
    PULSE            = 0x80,
    PULSE_SLOW       = 0x100,
    SPOT_LIGHT       = 0x200,
    SPOT_SHADOW      = 0x400
};


struct LightData {
	int32_t time;
	uint32_t radius;
	uint8_t color[4];
	uint32_t flags;
	float fallofExponent;
	float fov;
	uint32_t value;
	float weight;
};

struct Light : public Record {

	ObjectBounds objectBounds;
	ModelData modelData;
	formid script;
	std::string name;
	std::string largeIconFilename;
	std::string smallIconFilename;
	LightData lightData;
	float fadeValue;
	formid sound;

	Light(ESMReader& reader);

};

};
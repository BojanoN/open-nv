#pragma once
#include "record.hpp"
#include "subrecords/effect.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>

namespace ESM {

struct ObjectEffectData {
	uint32_t type;
	uint8_t unused_1[4]; //charge amount
	uint8_t unused_2[4]; //cost
	uint8_t flags;
	uint8_t unused_3[3]; 
};

struct ObjectEffect : public Record {

    std::string name;
    ObjectEffectData data;
    std::vector<Effect> effects;

    ObjectEffect(ESMReader& reader);
};

};

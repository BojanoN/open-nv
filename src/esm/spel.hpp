#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>

namespace ESM {

struct ActorEffectData {
	uint32_t type;
	uint8_t unused_1[4]; //cost
	uint8_t unused_2[4]; //level
	uint8_t flags;
	uint8_t unused_3[3]; 
};

struct ActorEffect : public Record {

    std::string name;
    ActorEffectData data;
    std::vector<Effect> effects;

    ActorEffect(ESMReader& reader);
};

};

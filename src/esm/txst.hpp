#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/decal_data.hpp"
#include "structs.hpp"
#include <string>
#include <cstdint>

namespace ESM {


struct TextureSet : public Record {

	ObjectBounds objectBounds;
	std::string baseImage_transparency;
	std::string normalMap_specular;
	std::string environmentMapMask;
	std::string glowMap;
	std::string parallaxMap;
	std::string environmentMap;
	DecalData decalData;
	uint16_t flags;

	TextureSet(ESMReader&);
	//virtual void load(ESMReader& reader);
};


};
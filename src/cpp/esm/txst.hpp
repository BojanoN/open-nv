#include "reader.hpp"
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct DecalData {
	float minWidth;
	float maxWidth;
	float minHeight;
	float maxHeight;
	float depth;
	float shininess;
	float parallaxScale;
	uint8_t parallaxPasses;
	uint8_t flags;
	uint8_t unused[2];
	uint8_t color[4];
};

struct TextureSet : public Record {

	std::string editorId;
	ObjectBounds objectBounds;
	std::string baseImage_transparency;
	std::string normalMap_specular;
	std::string environmentMapMask;
	std::string glowMap;
	std::string parallaxMap;
	std::string environmentMap;
	DecalData decalData;
	uint16_t flags;


	virtual void load(ESMReader& reader);
};


};
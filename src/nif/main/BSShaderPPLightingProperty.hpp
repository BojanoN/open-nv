
        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include <memory>
        #include <vector>
        #include <array>
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        struct BSShaderLightingProperty;
#include "BSShaderLightingProperty.hpp"
struct BSShaderTextureSet;
#include "BSShaderTextureSet.hpp"
#include <cstdint>

struct BSShaderPPLightingProperty : public BSShaderLightingProperty {
	NifPointer<BSShaderTextureSet> textureSet;
	float refractionStrength;
	int32_t refractionFirePeriod;
	float parallaxMaxPasses;
	float parallaxScale;


	BSShaderPPLightingProperty(NifReader& reader);


	virtual ~BSShaderPPLightingProperty();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

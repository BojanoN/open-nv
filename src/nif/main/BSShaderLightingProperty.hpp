
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
        struct BSShaderProperty;
#include "BSShaderProperty.hpp"
#include <cstdint>

struct BSShaderLightingProperty : public BSShaderProperty {
	NiEnums::TexClampMode textureClampMode;


	BSShaderLightingProperty(NifReader& reader);


	virtual ~BSShaderLightingProperty();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

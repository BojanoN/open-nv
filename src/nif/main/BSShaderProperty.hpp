
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
        struct NiShadeProperty;
#include "NiShadeProperty.hpp"
#include <cstdint>

struct BSShaderProperty : public NiShadeProperty {
	NiEnums::BSShaderType shaderType;
	NiEnums::BSShaderFlags shaderFlags;
	NiEnums::BSShaderFlags2 shaderFlags2;
	float environmentMapScale;


	BSShaderProperty(NifReader& reader);


	virtual ~BSShaderProperty();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

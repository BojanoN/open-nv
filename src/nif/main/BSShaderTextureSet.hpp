
        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        struct NiObject;
#include "NiObject.hpp"
#include <cstdint>

struct BSShaderTextureSet : public NiObject {
	uint32_t numTextures;
	char** textures;


	BSShaderTextureSet(NifReader& reader);


	virtual ~BSShaderTextureSet();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

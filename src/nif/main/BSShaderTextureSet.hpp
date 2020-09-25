
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
        struct NiObject;
#include "NiObject.hpp"
#include <cstdint>

struct BSShaderTextureSet : public NiObject {
	uint32_t numTextures;
	std::vector<std::string> textures;


	BSShaderTextureSet(NifReader& reader);


	virtual ~BSShaderTextureSet();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

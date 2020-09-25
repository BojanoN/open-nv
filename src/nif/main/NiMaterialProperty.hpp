
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
        struct NiProperty;
#include "NiProperty.hpp"
#include <cstdint>

struct NiMaterialProperty : public NiProperty {
	Color3 specularColor;
	Color3 emissiveColor;
	float glossiness;
	float alpha;
	float emissiveMult;


	NiMaterialProperty(NifReader& reader);


	virtual ~NiMaterialProperty();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

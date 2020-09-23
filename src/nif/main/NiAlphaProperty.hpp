
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

struct NiAlphaProperty : public NiProperty {
	AlphaFlags flags;
	uint8_t threshold;


	NiAlphaProperty(NifReader& reader);


	virtual ~NiAlphaProperty();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

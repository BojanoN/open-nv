
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
        struct NiExtraData;
#include "NiExtraData.hpp"
#include <cstdint>

struct NiIntegerExtraData : public NiExtraData {
	uint32_t integerData;


	NiIntegerExtraData(NifReader& reader);


	virtual ~NiIntegerExtraData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

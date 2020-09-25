
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
        struct NiIntegerExtraData;
#include "NiIntegerExtraData.hpp"
#include <cstdint>

struct BSXFlags : public NiIntegerExtraData {


	BSXFlags(NifReader& reader);


	virtual ~BSXFlags();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

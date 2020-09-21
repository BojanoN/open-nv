
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
        struct NiObjectNET;
#include "NiObjectNET.hpp"
#include <cstdint>

struct NiProperty : public NiObjectNET {


	NiProperty(NifReader& reader);


	virtual ~NiProperty();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};


        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        #include <cstdint>

struct NiObject {


	NiObject(NifReader& reader);


	virtual ~NiObject();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

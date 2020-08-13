
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

struct AbstractAdditionalGeometryData : public NiObject {


	AbstractAdditionalGeometryData(NifReader& reader);


	virtual ~AbstractAdditionalGeometryData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

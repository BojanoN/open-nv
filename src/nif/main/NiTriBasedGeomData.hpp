
        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        struct NiGeometryData;
#include "NiGeometryData.hpp"
#include <cstdint>

struct NiTriBasedGeomData : public NiGeometryData {
	uint16_t numTriangles;


	NiTriBasedGeomData(NifReader& reader);


	virtual ~NiTriBasedGeomData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};


        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        struct NiTriBasedGeomData;
#include "NiTriBasedGeomData.hpp"
#include <cstdint>

struct NiTriStripsData : public NiTriBasedGeomData {
	uint16_t numStrips;
	uint16_t* stripLengths;
	nif_bool_t hasPoints;
	uint16_t** points;


	NiTriStripsData(NifReader& reader);


	virtual ~NiTriStripsData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};


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
        struct NiTriBasedGeomData;
#include "NiTriBasedGeomData.hpp"
#include <cstdint>

struct NiTriStripsData : public NiTriBasedGeomData {
	uint16_t numStrips;
	std::vector<uint16_t> stripLengths;
	nif_bool_t hasPoints;
	std::vector<std::vector<uint16_t>> points;


	NiTriStripsData(NifReader& reader);


	virtual ~NiTriStripsData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

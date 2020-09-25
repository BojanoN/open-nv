
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
#include <vector>
#include <cstdint>

struct NiTriShapeData : public NiTriBasedGeomData {
	uint32_t numTrianglePoints;
	nif_bool_t hasTriangles;
	std::vector<Triangle> triangles;
	uint16_t numMatchGroups;
	std::vector<MatchGroup> matchGroups;


	NiTriShapeData(NifReader& reader);


	virtual ~NiTriShapeData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};


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
        struct NiGeometry;
#include "NiGeometry.hpp"
#include <cstdint>

struct NiTriBasedGeom : public NiGeometry {


	NiTriBasedGeom(NifReader& reader);


	virtual ~NiTriBasedGeom();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

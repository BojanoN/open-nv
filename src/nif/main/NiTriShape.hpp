
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
        struct NiTriBasedGeom;
#include "NiTriBasedGeom.hpp"
#include <cstdint>

struct NiTriShape : public NiTriBasedGeom {


	NiTriShape(NifReader& reader);


	virtual ~NiTriShape();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};


        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        struct NiAVObject;
#include "NiAVObject.hpp"
struct NiGeometryData;
#include "NiGeometryData.hpp"
struct NiSkinInstance;
#include "NiSkinInstance.hpp"
#include <cstdint>

struct NiGeometry : public NiAVObject {
	NifPointer<NiGeometryData> data;
	NifPointer<NiSkinInstance> skinInstance;
	MaterialData* materialData;


	NiGeometry(NifReader& reader);


	virtual ~NiGeometry();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

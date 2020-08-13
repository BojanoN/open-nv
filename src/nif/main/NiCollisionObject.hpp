
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
struct NiAVObject;
#include "NiAVObject.hpp"
#include <cstdint>

struct NiCollisionObject : public NiObject {
	NifPointer<NiAVObject> target;


	NiCollisionObject(NifReader& reader);


	virtual ~NiCollisionObject();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

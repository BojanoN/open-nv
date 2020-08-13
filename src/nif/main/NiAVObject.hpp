
        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        struct NiObjectNET;
#include "NiObjectNET.hpp"
struct NiProperty;
#include "NiProperty.hpp"
struct NiCollisionObject;
#include "NiCollisionObject.hpp"
#include <cstdint>

struct NiAVObject : public NiObjectNET {
	uint32_t flags;
	Vector3* translation;
	Matrix33* rotation;
	float scale;
	uint32_t numProperties;
	NifPointerList<NiProperty> properties;
	NifPointer<NiCollisionObject> collisionObject;


	NiAVObject(NifReader& reader);


	virtual ~NiAVObject();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};


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
struct NiNode;
#include "NiNode.hpp"
#include <cstdint>

struct NiDynamicEffect : public NiAVObject {
	nif_bool_t switchState;
	uint32_t numAffectedNodes;
	NifPointerList<NiNode> affectedNodes;


	NiDynamicEffect(NifReader& reader);


	virtual ~NiDynamicEffect();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

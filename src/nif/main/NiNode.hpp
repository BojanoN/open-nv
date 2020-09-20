
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
        struct NiAVObject;
#include "NiAVObject.hpp"
struct NiAVObject;
#include "NiAVObject.hpp"
struct NiDynamicEffect;
#include "NiDynamicEffect.hpp"
#include <cstdint>

struct NiNode : public NiAVObject {
	uint32_t numChildren;
	NifPointerList<NiAVObject> children;
	uint32_t numEffects;
	NifPointerList<NiDynamicEffect> effects;


	NiNode(NifReader& reader);


	virtual ~NiNode();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

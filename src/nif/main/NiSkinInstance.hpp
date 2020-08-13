
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
struct NiSkinData;
#include "NiSkinData.hpp"
struct NiSkinPartition;
#include "NiSkinPartition.hpp"
struct NiNode;
#include "NiNode.hpp"
struct NiNode;
#include "NiNode.hpp"
#include <cstdint>

struct NiSkinInstance : public NiObject {
	NifPointer<NiSkinData> data;
	NifPointer<NiSkinPartition> skinPartition;
	NifPointer<NiNode> skeletonRoot;
	uint32_t numBones;
	NifPointerList<NiNode> bones;


	NiSkinInstance(NifReader& reader);


	virtual ~NiSkinInstance();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

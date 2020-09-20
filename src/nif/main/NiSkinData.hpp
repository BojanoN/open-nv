
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
        struct NiObject;
#include "NiObject.hpp"
#include <vector>
#include <cstdint>

struct NiSkinData : public NiObject {
	std::shared_ptr<NiTransform> skinTransform;
	uint32_t numBones;
	uint8_t hasVertexWeights;
	std::vector<BoneData> boneList;


	NiSkinData(NifReader& reader);


	virtual ~NiSkinData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

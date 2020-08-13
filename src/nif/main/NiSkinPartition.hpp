
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
#include <vector>
#include <cstdint>

struct NiSkinPartition : public NiObject {
	uint32_t numPartitions;
	std::vector<SkinPartition> partitions;


	NiSkinPartition(NifReader& reader);


	virtual ~NiSkinPartition();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

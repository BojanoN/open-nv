
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
        struct NiSkinInstance;
#include "NiSkinInstance.hpp"
#include <vector>
#include <cstdint>

struct BSDismemberSkinInstance : public NiSkinInstance {
	uint32_t numPartitions;
	std::vector<BodyPartList> partitions;


	BSDismemberSkinInstance(NifReader& reader);


	virtual ~BSDismemberSkinInstance();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

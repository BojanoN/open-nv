
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
#include <cstdint>

struct NiExtraData : public NiObject {
	std::string name;


	NiExtraData(NifReader& reader);


	virtual ~NiExtraData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

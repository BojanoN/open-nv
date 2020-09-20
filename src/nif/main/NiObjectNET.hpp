
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
struct NiExtraData;
#include "NiExtraData.hpp"
struct NiTimeController;
#include "NiTimeController.hpp"
#include <cstdint>

struct NiObjectNET : public NiObject {
	std::string name;
	uint32_t numExtraDataList;
	NifPointerList<NiExtraData> extraDataList;
	NifPointer<NiTimeController> controller;


	NiObjectNET(NifReader& reader);


	virtual ~NiObjectNET();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};


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
struct NiTimeController;
#include "NiTimeController.hpp"
struct NiObjectNET;
#include "NiObjectNET.hpp"
#include <cstdint>

struct NiTimeController : public NiObject {
	NifPointer<NiTimeController> nextController;
	TimeControllerFlags flags;
	float frequency;
	float phase;
	float startTime;
	float stopTime;
	NifPointer<NiObjectNET> target;


	NiTimeController(NifReader& reader);


	virtual ~NiTimeController();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

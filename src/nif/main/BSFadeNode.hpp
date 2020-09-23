
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
        struct NiNode;
#include "NiNode.hpp"
#include <cstdint>

struct BSFadeNode : public NiNode {


	BSFadeNode(NifReader& reader);


	virtual ~BSFadeNode();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

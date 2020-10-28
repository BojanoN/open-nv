
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
        struct NiTriBasedGeom;
#include "NiTriBasedGeom.hpp"
namespace SceneGraph {
    class Node;
    class GroupNode;
};

#include <scene_graph/node.hpp>
#include <cstdint>

struct NiTriStrips : public NiTriBasedGeom {


	NiTriStrips(NifReader& reader);


	virtual ~NiTriStrips();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
        virtual Node* createGraphNode();
};

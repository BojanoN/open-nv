
#pragma once
#include <array>
#include <memory>
#include <vector>
#include "../nifpointer.hpp"
#include "../nifreader.hpp"
#include "bitfields.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "typedefs.hpp"
class NifData;
struct NiAVObject;
#include "NiAVObject.hpp"
struct NiAVObject;
#include "NiAVObject.hpp"
struct NiDynamicEffect;
#include "NiDynamicEffect.hpp"

namespace SceneGraph {
    class Node;
    class GroupNode;
};

#include <scene_graph/node.hpp>

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

  virtual Node* createGraphNode();
};

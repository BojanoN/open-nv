
#pragma once
#include "../nifpointer.hpp"
#include "../nifreader.hpp"
#include "bitfields.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "typedefs.hpp"
#include <array>
#include <memory>
#include <vector>
class NifData;
struct NiObject;
#include "NiObject.hpp"
#include <vector>
struct AbstractAdditionalGeometryData;
#include "AbstractAdditionalGeometryData.hpp"
namespace SceneGraph {
    class Node;
    class GroupNode;
};

#include <scene_graph/node.hpp>
#include <cstdint>

struct NiGeometryData : public NiObject {
    int32_t                                    groupID;
    uint16_t                                   numVertices;
    uint16_t                                   bsMaxVertices;
    uint8_t                                    keepFlags;
    uint8_t                                    compressFlags;
    nif_bool_t                                 hasVertices;
    std::vector<Vector3>                       vertices;
    BSGeometryDataFlags                        bsDataFlags;
    nif_bool_t                                 hasNormals;
    std::vector<Vector3>                       normals;
    std::vector<Vector3>                       tangents;
    std::vector<Vector3>                       bitangents;
    NiBound                                    boundingSphere;
    nif_bool_t                                 hasVertexColors;
    std::vector<Color4>                        vertexColors;
    std::vector<TexCoord>                      uvSets;
    NiEnums::ConsistencyType                   consistencyFlags;
    NifPointer<AbstractAdditionalGeometryData> additionalData;

    NiGeometryData(NifReader& reader);

    virtual ~NiGeometryData();
    static NiObject* create(NifReader& reader);
    virtual void     resolvePointers(NifData& data);

    virtual Node* createLeafNode(Vector3 translation, Matrix33 rotation, float scale, uint32_t numProperties, NodeProperty* properties) = 0;
};

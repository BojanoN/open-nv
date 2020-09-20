
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
struct AbstractAdditionalGeometryData;
#include "AbstractAdditionalGeometryData.hpp"
#include <cstdint>

struct NiGeometryData : public NiObject {
	int32_t groupID;
	uint16_t numVertices;
	uint16_t bsMaxVertices;
	uint8_t keepFlags;
	uint8_t compressFlags;
	nif_bool_t hasVertices;
	std::vector<Vector3> vertices;
	BSGeometryDataFlags bsDataFlags;
	nif_bool_t hasNormals;
	std::vector<Vector3> normals;
	std::vector<Vector3> tangents;
	std::vector<Vector3> bitangents;
	std::shared_ptr<NiBound> boundingSphere;
	nif_bool_t hasVertexColors;
	std::vector<Color4> vertexColors;
	std::vector<TexCoord> uvSets;
	NiEnums::ConsistencyType consistencyFlags;
	NifPointer<AbstractAdditionalGeometryData> additionalData;


	NiGeometryData(NifReader& reader);


	virtual ~NiGeometryData();
	static NiObject* create(NifReader& reader);
	virtual void resolvePointers(NifData& data);
};

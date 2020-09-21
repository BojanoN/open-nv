
    #pragma once
    #include "typedefs.hpp"
    #include "enums.hpp"
    #include "bitfields.hpp"
        #include "../nifreader.hpp"
#include "../nifdata.hpp"
#include "../nifpointer.hpp"
    #include <cstdint>
    #include <memory>
    #include <vector>
    #include <array>

struct Vector3 {
	float x;
	float y;
	float z;


	void load(NifReader& reader);


	~Vector3();
	void resolvePointers(NifData& data);
};

struct NiBound {
	Vector3 center;
	float radius;


	void load(NifReader& reader);


	~NiBound();
	void resolvePointers(NifData& data);
};

struct Color4 {
	float r;
	float g;
	float b;
	float a;


	void load(NifReader& reader);


	~Color4();
	void resolvePointers(NifData& data);
};

struct TexCoord {
	float u;
	float v;


	void load(NifReader& reader);


	~TexCoord();
	void resolvePointers(NifData& data);
};

    struct SizedString {
	uint32_t length;
	std::vector<int8_t> value;


	void load(NifReader& reader);


	~SizedString();
	void resolvePointers(NifData& data);
};

struct SizedString16 {
	uint16_t length;
	std::vector<int8_t> value;


	void load(NifReader& reader);


	~SizedString16();
	void resolvePointers(NifData& data);
};

struct string {
	uint32_t index;


	void load(NifReader& reader);


	~string();
	void resolvePointers(NifData& data);
};

template <typename T>
struct NiTFixedStringMapItem {
	uint32_t string;
	std::shared_ptr<T> value;


	void load(NifReader& reader);


	~NiTFixedStringMapItem();
	void resolvePointers(NifData& data);
};

template <typename T>
struct NiTFixedStringMap {
	uint32_t numStrings;
	std::vector<NiTFixedStringMapItem<T>> strings;


	void load(NifReader& reader);


	~NiTFixedStringMap();
	void resolvePointers(NifData& data);
};

struct ByteMatrix {
	uint32_t dataSize1;
	uint32_t dataSize2;
	std::vector<std::vector<uint8_t>> data;


	void load(NifReader& reader);


	~ByteMatrix();
	void resolvePointers(NifData& data);
};
struct Color3 {
	float r;
	float g;
	float b;


	void load(NifReader& reader);


	~Color3();
	void resolvePointers(NifData& data);
};

struct ByteColor3 {
	uint8_t r;
	uint8_t g;
	uint8_t b;


	void load(NifReader& reader);


	~ByteColor3();
	void resolvePointers(NifData& data);
};

struct Matrix33 {
	float m11;
	float m21;
	float m31;
	float m12;
	float m22;
	float m32;
	float m13;
	float m23;
	float m33;


	void load(NifReader& reader);


	~Matrix33();
	void resolvePointers(NifData& data);
};

struct Triangle;


struct Triangle {
	uint16_t v1;
	uint16_t v2;
	uint16_t v3;


	void load(NifReader& reader);


	~Triangle();
	void resolvePointers(NifData& data);
};

struct NiTransform {
	Matrix33 rotation;
	Vector3 translation;
	float scale;


	void load(NifReader& reader);


	~NiTransform();
	void resolvePointers(NifData& data);
};

struct BoneVertData {
	uint16_t index;
	float weight;


	void load(NifReader& reader);


	~BoneVertData();
	void resolvePointers(NifData& data);
};

struct BoneData {
	NiTransform skinTransform;
	NiBound boundingSphere;
	uint16_t numVertices;
	std::vector<BoneVertData> vertexWeights;


	void load(NifReader& reader, bool hasVertexWeights);


	~BoneData();
	void resolvePointers(NifData& data);
};

struct MaterialData {
	uint32_t numMaterials;
	std::vector<uint32_t> materialName;
	std::vector<int32_t> materialExtraData;
	int32_t activeMaterial;
	nif_bool_t materialNeedsUpdate;


	void load(NifReader& reader);


	~MaterialData();
	void resolvePointers(NifData& data);
};



struct MatchGroup {
	uint16_t numVertices;
	std::vector<uint16_t> vertexIndices;


	void load(NifReader& reader);


	~MatchGroup();
	void resolvePointers(NifData& data);
};

struct BodyPartList {
	NiEnums::BSPartFlag partFlag;
	NiEnums::BSDismemberBodyPartType bodyPart;


	void load(NifReader& reader);


	~BodyPartList();
	void resolvePointers(NifData& data);
};

struct SkinPartition {
	uint16_t numVertices;
	uint16_t numTriangles;
	uint16_t numBones;
	uint16_t numStrips;
	uint16_t numWeightsPerVertex;
	std::vector<uint16_t> bones;
	nif_bool_t hasVertexMap;
	std::vector<uint16_t> vertexMap;
	nif_bool_t hasVertexWeights;
	std::vector<std::vector<float>> vertexWeights;
	std::vector<uint16_t> stripLengths;
	nif_bool_t hasFaces;
	std::vector<std::vector<uint16_t>> strips;
	std::vector<Triangle> triangles;
	nif_bool_t hasBoneIndices;
	std::vector<std::vector<uint8_t>> boneIndices;


	void load(NifReader& reader);


	~SkinPartition();
	void resolvePointers(NifData& data);
};
#pragma once
#include "typedefs.hpp"
#include "enums.hpp"
//#include "utils.hpp"
#include "bitfields.hpp"
#include <cstdint>
#include <cstdio>
#include <vector>

#include "../nifreader.hpp"

class NifData;

struct Color3 {
	float r;
	float g;
	float b;


	Color3(NifReader& reader);


	~Color3();
	void resolvePointers(NifData& data);
};
/*
A vector in 3D space (x,y,z).
Size: 12
*/
struct Vector3 {
	float x;
	float y;
	float z;


	Vector3(NifReader& reader);


	~Vector3();
	void resolvePointers(NifData& data);
};


/*
A 3x3 rotation matrix; M^T M=identity, det(M)=1.    Stored in OpenGL column-major format.
Size: 36
*/
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


	Matrix33(NifReader& reader);


	~Matrix33();
	void resolvePointers(NifData& data);
};

/*
A color with alpha (red, green, blue, alpha).
Size: 16
*/
struct Color4 {
	float r;
	float g;
	float b;
	float a;


	Color4(NifReader& reader);


	~Color4();
	void resolvePointers(NifData& data);
};

/*
Texture coordinates (u,v). As in OpenGL; image origin is in the lower left corner.
Size: 8
*/
struct TexCoord {
	// Range: -100.0:100.0
	// First coordinate.
	float u;
	// Range: -100.0:100.0
	// Second coordinate.
	float v;


	TexCoord(NifReader& reader);
	void resolvePointers(NifData& data);
	~TexCoord();
};

/*
List of three vertex indices.
Size: 6
*/
struct Triangle {
	// First vertex index.
	uint16_t v1;
	// Second vertex index.
	uint16_t v2;
	// Third vertex index.
	uint16_t v3;


	Triangle(NifReader& reader);
	void resolvePointers(NifData& data);
	~Triangle();
};

/*
Skinning data for a submesh, optimized for hardware skinning. Part of NiSkinPartition.
*/
struct SkinPartition {
	uint16_t numVertices;
	uint16_t numTriangles;
	uint16_t numBones;
	uint16_t numStrips;
	uint16_t numWeightsPerVertex;
	uint16_t* bones;
	nif_bool_t hasVertexMap;
	uint16_t* vertexMap;
	nif_bool_t hasVertexWeights;
	float* vertexWeights;
	uint16_t* stripLengths;
	nif_bool_t hasFaces;
	uint16_t* strips;
	std::vector<Triangle> triangles;
	nif_bool_t hasBoneIndices;
	uint8_t* boneIndices;


	SkinPartition(NifReader& reader);


	~SkinPartition();
	void resolvePointers(NifData& data);
};

/*
NiSkinData::BoneVertData. A vertex and its weight.
Size: 6
*/
struct BoneVertData {
	// The vertex index, in the mesh.
	uint16_t index;
	// Range: 0.0:1.0
	// The vertex weight - between 0.0 and 1.0
	float weight;


	BoneVertData(NifReader& reader);
	void resolvePointers(NifData& data);
	~BoneVertData();
};
/*
A sphere.
*/
struct NiBound {
	// The sphere's center.
	Vector3* center;
	// The sphere's radius.
	float radius;


	NiBound(NifReader& reader);
	void resolvePointers(NifData& data);
	~NiBound();
};
/*

Size: 52
*/
struct NiTransform {
	// The rotation part of the transformation matrix.
	Matrix33* rotation;
	// The translation vector.
	Vector3* translation;
	// Scaling part (only uniform scaling is supported).
	float scale = 1.0;


	NiTransform(NifReader& reader);
	void resolvePointers(NifData& data);
	~NiTransform();
};
/*
NiSkinData::BoneData. Skinning data component.
*/
struct BoneData {
	NiTransform* skinTransform;
	NiBound* boundingSphere;
	uint16_t numVertices;
	std::vector<BoneVertData> vertexWeights;


	BoneData(NifReader& reader);


	~BoneData();
	void resolvePointers(NifData& data);
};

/*

*/
struct MaterialData {
	uint32_t numMaterials;
	// The name of the material.
	// Size: numMaterials
	uint32_t* materialName;
	// Extra data associated with the material. A value of -1 means the material is the default implementation.
	// Size: numMaterials
	int32_t* materialExtraData;
	// The index of the currently active material.
	int32_t activeMaterial = -1;
	// Whether the materials for this object always needs to be updated before rendering with them.
	nif_bool_t materialNeedsUpdate;


	MaterialData(NifReader& reader);
	void resolvePointers(NifData& data);
	~MaterialData();
};


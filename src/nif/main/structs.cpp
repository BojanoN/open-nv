#include "structs.hpp"
Vector3::Vector3(NifReader& reader) {
	reader.read(&x, sizeof(float), 1);
	reader.read(&y, sizeof(float), 1);
	reader.read(&z, sizeof(float), 1);
}
Vector3::~Vector3() {
}
void Vector3::resolvePointers(NifData& data) {
}

Matrix33::Matrix33(NifReader& reader) {
	reader.read(&m11, sizeof(float), 1);
	reader.read(&m21, sizeof(float), 1);
	reader.read(&m31, sizeof(float), 1);
	reader.read(&m12, sizeof(float), 1);
	reader.read(&m22, sizeof(float), 1);
	reader.read(&m32, sizeof(float), 1);
	reader.read(&m13, sizeof(float), 1);
	reader.read(&m23, sizeof(float), 1);
	reader.read(&m33, sizeof(float), 1);
}
Matrix33::~Matrix33() {
}
void Matrix33::resolvePointers(NifData& data) {
}

Color3::Color3(NifReader& reader) {
	reader.read(&r, sizeof(float), 1);
	reader.read(&g, sizeof(float), 1);
	reader.read(&b, sizeof(float), 1);
}
Color3::~Color3() {
}
void Color3::resolvePointers(NifData& data) {
}

Color4::Color4(NifReader& reader) {
	reader.read(&r, sizeof(float), 1);
	reader.read(&g, sizeof(float), 1);
	reader.read(&b, sizeof(float), 1);
	reader.read(&a, sizeof(float), 1);
}

Color4::~Color4() {
}
void Color4::resolvePointers(NifData& data) {
}

TexCoord::TexCoord(NifReader& reader) {
	reader.read(&u, sizeof(float), 1);
	reader.read(&v, sizeof(float), 1);
}

TexCoord::~TexCoord() {
}
void TexCoord::resolvePointers(NifData& data) {
}

SkinPartition::SkinPartition(NifReader& reader) {
	reader.read(&numVertices, sizeof(uint16_t), 1);
	reader.read(&numTriangles, sizeof(uint16_t), 1);
	reader.read(&numBones, sizeof(uint16_t), 1);
	reader.read(&numStrips, sizeof(uint16_t), 1);
	reader.read(&numWeightsPerVertex, sizeof(uint16_t), 1);
	bones = new uint16_t[numBones];
	reader.read(bones, sizeof(uint16_t), numBones);
	reader.read(&hasVertexMap, sizeof(nif_bool_t), 1);
if(hasVertexMap && numVertices != 0) {
	vertexMap = new uint16_t[numVertices];
	reader.read(vertexMap, sizeof(uint16_t), numVertices);
}
	reader.read(&hasVertexWeights, sizeof(nif_bool_t), 1);
if(hasVertexWeights && numVertices != 0) {
	vertexWeights = new float[numVertices];
	reader.read(vertexWeights, sizeof(float), numVertices);
}
	stripLengths = new uint16_t[numStrips];
	reader.read(stripLengths, sizeof(uint16_t), numStrips);
	reader.read(&hasFaces, sizeof(nif_bool_t), 1);
if((hasFaces)&&(numStrips!=0) && numStrips != 0) {
	strips = new uint16_t[numStrips];
	reader.read(strips, sizeof(uint16_t), numStrips);
}
if((hasFaces)&&(numStrips==0) && numTriangles != 0) {
	triangles.reserve(numTriangles);
	for(unsigned int i = 0; i < numTriangles; i++) {
		triangles.emplace_back(reader);
	}
}
	reader.read(&hasBoneIndices, sizeof(nif_bool_t), 1);
if(hasBoneIndices && numVertices != 0) {
	boneIndices = new uint8_t[numVertices];
	reader.read(boneIndices, sizeof(uint8_t), numVertices);
}
}
SkinPartition::~SkinPartition() {
	delete[]bones;
if(hasVertexMap && numVertices != 0) {
	delete[]vertexMap;
}
if(hasVertexWeights && numVertices != 0) {
	delete[]vertexWeights;
}
	delete[]stripLengths;
if((hasFaces)&&(numStrips!=0) && numStrips != 0) {
	delete[]strips;
}
if((hasFaces)&&(numStrips==0) && numTriangles != 0) {
}
if(hasBoneIndices && numVertices != 0) {
	delete[]boneIndices;
}
}
void SkinPartition::resolvePointers(NifData& data) {
	for(unsigned int i = 0; i < numTriangles; i++) {
		triangles[i].resolvePointers(data);
	}
}


Triangle::Triangle(NifReader& reader) {
	reader.read(&v1, sizeof(uint16_t), 1);
	reader.read(&v2, sizeof(uint16_t), 1);
	reader.read(&v3, sizeof(uint16_t), 1);
}

Triangle::~Triangle() {
}
void Triangle::resolvePointers(NifData& data) {
}


BoneData::BoneData(NifReader& reader) {
	skinTransform = new NiTransform(reader);
	boundingSphere = new NiBound(reader);
	reader.read(&numVertices, sizeof(uint16_t), 1);
if(0!=0 && numVertices != 0) {
	vertexWeights.reserve(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexWeights.emplace_back(reader);
	}
}
}
BoneData::~BoneData() {
	delete skinTransform;
	delete boundingSphere;
if(0!=0 && numVertices != 0) {
}
}
void BoneData::resolvePointers(NifData& data) {
	skinTransform->resolvePointers(data);
	boundingSphere->resolvePointers(data);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexWeights[i].resolvePointers(data);
	}
}

BoneVertData::~BoneVertData() {
}
void BoneVertData::resolvePointers(NifData& data) {
}


MaterialData::MaterialData(NifReader& reader) {
	reader.read(&numMaterials, sizeof(uint32_t), 1);
	if(numMaterials != 0) {
	materialName = new uint32_t[numMaterials];
	reader.read(materialName, sizeof(uint32_t), numMaterials);
	}
	if(numMaterials != 0) {
	materialExtraData = new int32_t[numMaterials];
	reader.read(materialExtraData, sizeof(int32_t), numMaterials);
	}
	reader.read(&activeMaterial, sizeof(int32_t), 1);
	reader.read(&materialNeedsUpdate, sizeof(nif_bool_t), 1);
}

MaterialData::~MaterialData() {
	if(numMaterials != 0) {
	 delete[] materialName;
	}
	if(numMaterials != 0) {
	 delete[] materialExtraData;
	}
}
void MaterialData::resolvePointers(NifData& data) {
}

NiBound::NiBound(NifReader& reader) {
	center = new Vector3(reader);
	reader.read(&radius, sizeof(float), 1);
}

NiBound::~NiBound() {
	delete center;
}
void NiBound::resolvePointers(NifData& data) {
	center->resolvePointers(data);
}


NiTransform::NiTransform(NifReader& reader) {
	rotation = new Matrix33(reader);
	translation = new Vector3(reader);
	reader.read(&scale, sizeof(float), 1);
}

NiTransform::~NiTransform() {
	delete rotation;
	delete translation;
}
void NiTransform::resolvePointers(NifData& data) {
	rotation->resolvePointers(data);
	translation->resolvePointers(data);
}

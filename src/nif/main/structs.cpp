#include "structs.hpp"


void Vector3::load(NifReader& reader) {
	reader.read(&x, sizeof(float), 1);
	reader.read(&y, sizeof(float), 1);
	reader.read(&z, sizeof(float), 1);
}
Vector3::~Vector3() {
}
void Vector3::resolvePointers(NifData& data) {
}
void TexCoord::load(NifReader& reader) {
	reader.read(&u, sizeof(float), 1);
	reader.read(&v, sizeof(float), 1);
}
TexCoord::~TexCoord() {
}
void TexCoord::resolvePointers(NifData& data) {
}

void SizedString::load(NifReader& reader) {
	reader.read(&length, sizeof(uint32_t), 1);
	value.resize(length);
	reader.read(&value[0], sizeof(int8_t), length);
}
SizedString::~SizedString() {
}
void SizedString::resolvePointers(NifData& data) {
}
void SizedString16::load(NifReader& reader) {
	reader.read(&length, sizeof(uint16_t), 1);
	value.resize(length);
	reader.read(&value[0], sizeof(int8_t), length);
}
SizedString16::~SizedString16() {
}
void SizedString16::resolvePointers(NifData& data) {
}
void string::load(NifReader& reader) {
	reader.read(&index, sizeof(uint32_t), 1);
}
string::~string() {
}
void string::resolvePointers(NifData& data) {
}
template <typename T>
 void NiTFixedStringMapItem<T>::load(NifReader& reader) {
	reader.read(&string, sizeof(uint32_t), 1);
	value = std::make_shared<T>(reader);
}
template <typename T>
 NiTFixedStringMapItem<T>::~NiTFixedStringMapItem() {
}
template <typename T>
void NiTFixedStringMapItem<T>::resolvePointers(NifData& data) {
	value->resolvePointers(data);
}
template <typename T>
 void NiTFixedStringMap<T>::load(NifReader& reader) {
	reader.read(&numStrings, sizeof(uint32_t), 1);
	strings.resize(numStrings);
	for(unsigned int i = 0; i < numStrings; i++) {
		strings[i].load(reader);
	}
}
template <typename T>
 NiTFixedStringMap<T>::~NiTFixedStringMap() {
}
template <typename T>
void NiTFixedStringMap<T>::resolvePointers(NifData& data) {
	for(unsigned int i = 0; i < numStrings; i++) {
		strings[i].resolvePointers(data);
	}
}

ByteMatrix::~ByteMatrix() {
}
void ByteMatrix::resolvePointers(NifData& data) {
}
void Color3::load(NifReader& reader) {
	reader.read(&r, sizeof(float), 1);
	reader.read(&g, sizeof(float), 1);
	reader.read(&b, sizeof(float), 1);
}
Color3::~Color3() {
}
void Color3::resolvePointers(NifData& data) {
}
void ByteColor3::load(NifReader& reader) {
	reader.read(&r, sizeof(uint8_t), 1);
	reader.read(&g, sizeof(uint8_t), 1);
	reader.read(&b, sizeof(uint8_t), 1);
}
ByteColor3::~ByteColor3() {
}
void ByteColor3::resolvePointers(NifData& data) {
}
void Color4::load(NifReader& reader) {
	reader.read(&r, sizeof(float), 1);
	reader.read(&g, sizeof(float), 1);
	reader.read(&b, sizeof(float), 1);
	reader.read(&a, sizeof(float), 1);
}
Color4::~Color4() {
}
void Color4::resolvePointers(NifData& data) {
}

void Matrix33::load(NifReader& reader) {
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

void SkinPartition::load(NifReader& reader) {
	reader.read(&numVertices, sizeof(uint16_t), 1);
	reader.read(&numTriangles, sizeof(uint16_t), 1);
	reader.read(&numBones, sizeof(uint16_t), 1);
	reader.read(&numStrips, sizeof(uint16_t), 1);
	reader.read(&numWeightsPerVertex, sizeof(uint16_t), 1);
	bones.resize(numBones);
	reader.read(&bones[0], sizeof(uint16_t), numBones);
	reader.read(&hasVertexMap, sizeof(nif_bool_t), 1);
if(hasVertexMap && numVertices != 0) {
	vertexMap.resize(numVertices);
	reader.read(&vertexMap[0], sizeof(uint16_t), numVertices);
}
	reader.read(&hasVertexWeights, sizeof(nif_bool_t), 1);
if(hasVertexWeights && numVertices != 0) {
	vertexWeights.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexWeights[i].resize(numWeightsPerVertex);
		reader.read(&vertexWeights[i][0], sizeof(float), numWeightsPerVertex);
	}
}
	stripLengths.resize(numStrips);
	reader.read(&stripLengths[0], sizeof(uint16_t), numStrips);
	reader.read(&hasFaces, sizeof(nif_bool_t), 1);
if(hasFaces && numStrips != 0 && numStrips != 0) {
	strips.resize(numStrips);
	for(unsigned int i = 0; i < numStrips; i++) {
		strips[i].resize(stripLengths[i]);
		reader.read(&strips[i][0], sizeof(uint16_t), stripLengths[i]);
	}
}
if(hasFaces && numStrips == 0 && numTriangles != 0) {
	triangles.resize(numTriangles);
	for(unsigned int i = 0; i < numTriangles; i++) {
		triangles[i].load(reader);
	}
}
	reader.read(&hasBoneIndices, sizeof(nif_bool_t), 1);
if(hasBoneIndices && numVertices != 0) {
	boneIndices.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		boneIndices[i].resize(numWeightsPerVertex);
		reader.read(&boneIndices[i][0], sizeof(uint8_t), numWeightsPerVertex);
	}
}
}
SkinPartition::~SkinPartition() {
if(hasVertexMap && numVertices != 0) {
}
if(hasVertexWeights && numVertices != 0) {
}
if(hasFaces && numStrips != 0 && numStrips != 0) {
}
if(hasFaces && numStrips == 0 && numTriangles != 0) {
}
if(hasBoneIndices && numVertices != 0) {
}
}
void SkinPartition::resolvePointers(NifData& data) {
	for(unsigned int i = 0; i < numTriangles; i++) {
		triangles[i].resolvePointers(data);
	}
}

void Triangle::load(NifReader& reader) {
	reader.read(&v1, sizeof(uint16_t), 1);
	reader.read(&v2, sizeof(uint16_t), 1);
	reader.read(&v3, sizeof(uint16_t), 1);
}
Triangle::~Triangle() {
}
void Triangle::resolvePointers(NifData& data) {
}

void MaterialData::load(NifReader& reader) {
	reader.read(&numMaterials, sizeof(uint32_t), 1);
	materialName.resize(numMaterials);
	reader.read(&materialName[0], sizeof(uint32_t), numMaterials);
	materialExtraData.resize(numMaterials);
	reader.read(&materialExtraData[0], sizeof(int32_t), numMaterials);
	reader.read(&activeMaterial, sizeof(int32_t), 1);
	reader.read(&materialNeedsUpdate, sizeof(nif_bool_t), 1);
}
MaterialData::~MaterialData() {
}
void MaterialData::resolvePointers(NifData& data) {
}

void BoneData::load(NifReader& reader, bool hasVertexWeights) {
	skinTransform.load(reader);
	boundingSphere.load(reader);
	reader.read(&numVertices, sizeof(uint16_t), 1);
if(hasVertexWeights && numVertices != 0) {
	vertexWeights.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexWeights[i].load(reader);
	}
}
}
BoneData::~BoneData() {
if(0 != 0 && numVertices != 0) {
}
}
void BoneData::resolvePointers(NifData& data) {
	skinTransform.resolvePointers(data);
	boundingSphere.resolvePointers(data);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexWeights[i].resolvePointers(data);
	}
}

void NiTransform::load(NifReader& reader) {
	rotation.load(reader);
	translation.load(reader);
	reader.read(&scale, sizeof(float), 1);
}
NiTransform::~NiTransform() {
}
void NiTransform::resolvePointers(NifData& data) {
	rotation.resolvePointers(data);
	translation.resolvePointers(data);
}

void BoneVertData::load(NifReader& reader) {
	reader.read(&index, sizeof(uint16_t), 1);
	reader.read(&weight, sizeof(float), 1);
}
BoneVertData::~BoneVertData() {
}
void BoneVertData::resolvePointers(NifData& data) {
}

void NiBound::load(NifReader& reader) {
	center.load(reader);
	reader.read(&radius, sizeof(float), 1);
}
NiBound::~NiBound() {
}
void NiBound::resolvePointers(NifData& data) {
	center.resolvePointers(data);
}

void MatchGroup::load(NifReader& reader) {
	reader.read(&numVertices, sizeof(uint16_t), 1);
	vertexIndices.resize(numVertices);
	reader.read(&vertexIndices[0], sizeof(uint16_t), numVertices);
}
MatchGroup::~MatchGroup() {
}
void MatchGroup::resolvePointers(NifData& data) {
}

void BodyPartList::load(NifReader& reader) {
	reader.read(&partFlag, sizeof(NiEnums::BSPartFlag), 1);
	reader.read(&bodyPart, sizeof(NiEnums::BSDismemberBodyPartType), 1);
}
BodyPartList::~BodyPartList() {
}
void BodyPartList::resolvePointers(NifData& data) {
}

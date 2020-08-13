#include "NiGeometryData.hpp"
NiGeometryData::NiGeometryData(NifReader& reader) : NiObject(reader) {
	reader.read(&groupID, sizeof(int32_t), 1);
	reader.read(&numVertices, sizeof(uint16_t), 1);
	reader.read(&bsMaxVertices, sizeof(uint16_t), 1);
	reader.read(&keepFlags, sizeof(uint8_t), 1);
	reader.read(&compressFlags, sizeof(uint8_t), 1);
	reader.read(&hasVertices, sizeof(nif_bool_t), 1);
if(hasVertices && numVertices != 0) {
	vertices.reserve(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertices.emplace_back(reader);
	}
}
	reader.read(&bsDataFlags, sizeof(BSGeometryDataFlags), 1);
	reader.read(&hasNormals, sizeof(nif_bool_t), 1);
if(hasNormals && numVertices != 0) {
	normals.reserve(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		normals.emplace_back(reader);
	}
}
if(hasNormals && bsDataFlags & 4096 && numVertices != 0) {
	tangents.reserve(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		tangents.emplace_back(reader);
	}
}
if(hasNormals && bsDataFlags & 4096 && numVertices != 0) {
	bitangents.reserve(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		bitangents.emplace_back(reader);
	}
}
	boundingSphere = new NiBound(reader);
	reader.read(&hasVertexColors, sizeof(nif_bool_t), 1);
if(hasVertexColors && numVertices != 0) {
	vertexColors.reserve(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexColors.emplace_back(reader);
	}
}
if(bsDataFlags & 1) {
	uvSets = new TexCoord(reader);
}
	reader.read(&consistencyFlags, sizeof(NiEnums::ConsistencyType), 1);
additionalData.load(reader);
}
NiObject* NiGeometryData::create(NifReader& reader) {
	return new NiGeometryData(reader);
}
NiGeometryData::~NiGeometryData() {
if(hasVertices && numVertices != 0) {
}
if(hasNormals && numVertices != 0) {
}
if(hasNormals && bsDataFlags & 4096 && numVertices != 0) {
}
if(hasNormals && bsDataFlags & 4096 && numVertices != 0) {
}
	delete boundingSphere;
if(hasVertexColors && numVertices != 0) {
}
if(bsDataFlags & 1) {
	delete uvSets;
}
}
void NiGeometryData::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertices[i].resolvePointers(data);
	}
	for(unsigned int i = 0; i < numVertices; i++) {
		normals[i].resolvePointers(data);
	}
	for(unsigned int i = 0; i < numVertices; i++) {
		tangents[i].resolvePointers(data);
	}
	for(unsigned int i = 0; i < numVertices; i++) {
		bitangents[i].resolvePointers(data);
	}
	boundingSphere->resolvePointers(data);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexColors[i].resolvePointers(data);
	}
	uvSets->resolvePointers(data);
this->additionalData.resolve(data);
}

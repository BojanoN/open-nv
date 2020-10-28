#include "NiGeometryData.hpp"
NiGeometryData::NiGeometryData(NifReader& reader) : NiObject(reader) {
	reader.read(&groupID, sizeof(int32_t), 1);
	reader.read(&numVertices, sizeof(uint16_t), 1);
	reader.read(&keepFlags, sizeof(uint8_t), 1);
	reader.read(&compressFlags, sizeof(uint8_t), 1);
	reader.read(&hasVertices, sizeof(nif_bool_t), 1);
if(hasVertices && numVertices != 0) {
	vertices.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertices[i].load(reader);
	}
}
	uint16_t bsDataFlagsData;
	reader.read(&bsDataFlagsData, sizeof(uint16_t), 1);
	bsDataFlags = bsDataFlagsData;
	reader.read(&hasNormals, sizeof(nif_bool_t), 1);
if(hasNormals && numVertices != 0) {
	normals.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		normals[i].load(reader);
	}
}
if(hasNormals && bsDataFlags & 4096 && numVertices != 0) {
	tangents.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		tangents[i].load(reader);
	}
}
if(hasNormals && bsDataFlags & 4096 && numVertices != 0) {
	bitangents.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		bitangents[i].load(reader);
	}
}
	boundingSphere.load(reader);
	reader.read(&hasVertexColors, sizeof(nif_bool_t), 1);
if(hasVertexColors && numVertices != 0) {
	vertexColors.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexColors[i].load(reader);
	}
}
if(bsDataFlags & 1 && numVertices != 0) {
	uvSets.resize(numVertices);
	for(unsigned int i = 0; i < numVertices; i++) {
		uvSets[i].load(reader);
	}
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
if(hasVertexColors && numVertices != 0) {
}
if(bsDataFlags & 1 && numVertices != 0) {
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
	boundingSphere.resolvePointers(data);
	for(unsigned int i = 0; i < numVertices; i++) {
		vertexColors[i].resolvePointers(data);
	}
	for(unsigned int i = 0; i < numVertices; i++) {
		uvSets[i].resolvePointers(data);
	}
this->additionalData.resolve(data);
}


Node* NiGeometryData::createGeometryData() {
	return new GeometryData(numVertices, &vertices[0], hasNormals ? &normals[0] : nullptr, hasNormals ? &tangents[0] : nullptr, hasNormals ? &bitangents[0] : nullptr, &uvSets[0], hasVertexColors ? &vertexColors[0] : nullptr);
}
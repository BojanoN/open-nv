#include "NiTriShapeData.hpp"
NiTriShapeData::NiTriShapeData(NifReader& reader) : NiTriBasedGeomData(reader) {
	reader.read(&numTrianglePoints, sizeof(uint32_t), 1);
	reader.read(&hasTriangles, sizeof(nif_bool_t), 1);
if(hasTriangles && numTriangles != 0) {
	triangles.resize(numTriangles);
	for(unsigned int i = 0; i < numTriangles; i++) {
		triangles[i].load(reader);
	}
}
	reader.read(&numMatchGroups, sizeof(uint16_t), 1);
	matchGroups.resize(numMatchGroups);
	for(unsigned int i = 0; i < numMatchGroups; i++) {
		matchGroups[i].load(reader);
	}
}
NiObject* NiTriShapeData::create(NifReader& reader) {
	return new NiTriShapeData(reader);
}
NiTriShapeData::~NiTriShapeData() {
if(hasTriangles && numTriangles != 0) {
}
}
void NiTriShapeData::resolvePointers(NifData& data) {
	NiTriBasedGeomData::resolvePointers(data);
	for(unsigned int i = 0; i < numTriangles; i++) {
		triangles[i].resolvePointers(data);
	}
	for(unsigned int i = 0; i < numMatchGroups; i++) {
		matchGroups[i].resolvePointers(data);
	}
}

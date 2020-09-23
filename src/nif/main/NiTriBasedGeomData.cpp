#include "NiTriBasedGeomData.hpp"
NiTriBasedGeomData::NiTriBasedGeomData(NifReader& reader) : NiGeometryData(reader) {
	reader.read(&numTriangles, sizeof(uint16_t), 1);
}
NiObject* NiTriBasedGeomData::create(NifReader& reader) {
	return new NiTriBasedGeomData(reader);
}
NiTriBasedGeomData::~NiTriBasedGeomData() {
}
void NiTriBasedGeomData::resolvePointers(NifData& data) {
	NiGeometryData::resolvePointers(data);
}

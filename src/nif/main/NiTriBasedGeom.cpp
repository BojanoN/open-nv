#include "NiTriBasedGeom.hpp"
NiTriBasedGeom::NiTriBasedGeom(NifReader& reader) : NiGeometry(reader) {
}
NiObject* NiTriBasedGeom::create(NifReader& reader) {
	return new NiTriBasedGeom(reader);
}
NiTriBasedGeom::~NiTriBasedGeom() {
}
void NiTriBasedGeom::resolvePointers(NifData& data) {
	NiGeometry::resolvePointers(data);
}

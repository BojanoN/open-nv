#include "NiTriShape.hpp"
NiTriShape::NiTriShape(NifReader& reader) : NiTriBasedGeom(reader) {
}
NiObject* NiTriShape::create(NifReader& reader) {
	return new NiTriShape(reader);
}
NiTriShape::~NiTriShape() {
}
void NiTriShape::resolvePointers(NifData& data) {
	NiTriBasedGeom::resolvePointers(data);
}

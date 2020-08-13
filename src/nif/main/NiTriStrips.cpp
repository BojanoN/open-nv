#include "NiTriStrips.hpp"
NiTriStrips::NiTriStrips(NifReader& reader) : NiTriBasedGeom(reader) {
}
NiObject* NiTriStrips::create(NifReader& reader) {
	return new NiTriStrips(reader);
}
NiTriStrips::~NiTriStrips() {
}
void NiTriStrips::resolvePointers(NifData& data) {
	NiTriBasedGeom::resolvePointers(data);
}

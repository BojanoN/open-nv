#include "NiGeometry.hpp"
NiGeometry::NiGeometry(NifReader& reader) : NiAVObject(reader) {
data.load(reader);
skinInstance.load(reader);
	materialData.load(reader);
}
NiObject* NiGeometry::create(NifReader& reader) {
	return new NiGeometry(reader);
}
NiGeometry::~NiGeometry() {
}
void NiGeometry::resolvePointers(NifData& data) {
	NiAVObject::resolvePointers(data);
this->data.resolve(data);
this->skinInstance.resolve(data);
	materialData.resolvePointers(data);
}

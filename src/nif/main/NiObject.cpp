#include "NiObject.hpp"
NiObject::NiObject(NifReader& reader) {
}
NiObject* NiObject::create(NifReader& reader) {
	return new NiObject(reader);
}
NiObject::~NiObject() {
}
void NiObject::resolvePointers(NifData& data) {
}

#include "NiCollisionObject.hpp"
NiCollisionObject::NiCollisionObject(NifReader& reader) : NiObject(reader) {
target.load(reader);
}
NiObject* NiCollisionObject::create(NifReader& reader) {
	return new NiCollisionObject(reader);
}
NiCollisionObject::~NiCollisionObject() {
}
void NiCollisionObject::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
this->target.resolve(data);
}

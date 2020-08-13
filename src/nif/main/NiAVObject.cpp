#include "NiAVObject.hpp"
NiAVObject::NiAVObject(NifReader& reader) : NiObjectNET(reader) {
	reader.read(&flags, sizeof(uint32_t), 1);
	translation = new Vector3(reader);
	rotation = new Matrix33(reader);
	reader.read(&scale, sizeof(float), 1);
	reader.read(&numProperties, sizeof(uint32_t), 1);
properties.load(reader, numProperties);
collisionObject.load(reader);
}
NiObject* NiAVObject::create(NifReader& reader) {
	return new NiAVObject(reader);
}
NiAVObject::~NiAVObject() {
	delete translation;
	delete rotation;
}
void NiAVObject::resolvePointers(NifData& data) {
	NiObjectNET::resolvePointers(data);
	translation->resolvePointers(data);
	rotation->resolvePointers(data);
this->properties.resolve(data);
this->collisionObject.resolve(data);
}

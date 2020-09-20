#include "NiAVObject.hpp"
NiAVObject::NiAVObject(NifReader& reader) : NiObjectNET(reader) {
	reader.read(&flags, sizeof(uint32_t), 1);
	translation = std::make_shared<Vector3>();
	translation->load(reader);
	rotation = std::make_shared<Matrix33>();
	rotation->load(reader);
	reader.read(&scale, sizeof(float), 1);
	reader.read(&numProperties, sizeof(uint32_t), 1);
properties.load(reader, numProperties);
collisionObject.load(reader);
}
NiObject* NiAVObject::create(NifReader& reader) {
	return new NiAVObject(reader);
}
NiAVObject::~NiAVObject() {
}
void NiAVObject::resolvePointers(NifData& data) {
	NiObjectNET::resolvePointers(data);
	translation->resolvePointers(data);
	rotation->resolvePointers(data);
this->properties.resolve(data);
this->collisionObject.resolve(data);
}

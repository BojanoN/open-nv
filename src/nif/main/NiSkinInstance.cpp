#include "NiSkinInstance.hpp"
NiSkinInstance::NiSkinInstance(NifReader& reader) : NiObject(reader) {
data.load(reader);
skinPartition.load(reader);
skeletonRoot.load(reader);
	reader.read(&numBones, sizeof(uint32_t), 1);
bones.load(reader, numBones);
}
NiObject* NiSkinInstance::create(NifReader& reader) {
	return new NiSkinInstance(reader);
}
NiSkinInstance::~NiSkinInstance() {
}
void NiSkinInstance::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
this->data.resolve(data);
this->skinPartition.resolve(data);
this->skeletonRoot.resolve(data);
this->bones.resolve(data);
}

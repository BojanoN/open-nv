#include "NiSkinData.hpp"
NiSkinData::NiSkinData(NifReader& reader) : NiObject(reader) {
	skinTransform = new NiTransform(reader);
	reader.read(&numBones, sizeof(uint32_t), 1);
	reader.read(&hasVertexWeights, sizeof(uint8_t), 1);
	boneList.reserve(numBones);
	for(unsigned int i = 0; i < numBones; i++) {
		boneList.emplace_back(reader);
	}
}
NiObject* NiSkinData::create(NifReader& reader) {
	return new NiSkinData(reader);
}
NiSkinData::~NiSkinData() {
	delete skinTransform;
}
void NiSkinData::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
	skinTransform->resolvePointers(data);
	for(unsigned int i = 0; i < numBones; i++) {
		boneList[i].resolvePointers(data);
	}
}

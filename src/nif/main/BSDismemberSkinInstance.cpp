#include "BSDismemberSkinInstance.hpp"
BSDismemberSkinInstance::BSDismemberSkinInstance(NifReader& reader) : NiSkinInstance(reader) {
	reader.read(&numPartitions, sizeof(uint32_t), 1);
	partitions.resize(numPartitions);
	for(unsigned int i = 0; i < numPartitions; i++) {
		partitions[i].load(reader);
	}
}
NiObject* BSDismemberSkinInstance::create(NifReader& reader) {
	return new BSDismemberSkinInstance(reader);
}
BSDismemberSkinInstance::~BSDismemberSkinInstance() {
}
void BSDismemberSkinInstance::resolvePointers(NifData& data) {
	NiSkinInstance::resolvePointers(data);
	for(unsigned int i = 0; i < numPartitions; i++) {
		partitions[i].resolvePointers(data);
	}
}

#include "NiSkinPartition.hpp"
NiSkinPartition::NiSkinPartition(NifReader& reader) : NiObject(reader) {
	reader.read(&numPartitions, sizeof(uint32_t), 1);
	partitions.reserve(numPartitions);
	for(unsigned int i = 0; i < numPartitions; i++) {
		partitions.emplace_back(reader);
	}
}
NiObject* NiSkinPartition::create(NifReader& reader) {
	return new NiSkinPartition(reader);
}
NiSkinPartition::~NiSkinPartition() {
}
void NiSkinPartition::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
	for(unsigned int i = 0; i < numPartitions; i++) {
		partitions[i].resolvePointers(data);
	}
}

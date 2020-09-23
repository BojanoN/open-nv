#include "NiIntegerExtraData.hpp"
NiIntegerExtraData::NiIntegerExtraData(NifReader& reader) : NiExtraData(reader) {
	reader.read(&integerData, sizeof(uint32_t), 1);
}
NiObject* NiIntegerExtraData::create(NifReader& reader) {
	return new NiIntegerExtraData(reader);
}
NiIntegerExtraData::~NiIntegerExtraData() {
}
void NiIntegerExtraData::resolvePointers(NifData& data) {
	NiExtraData::resolvePointers(data);
}

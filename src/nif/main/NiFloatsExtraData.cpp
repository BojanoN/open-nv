#include "NiFloatsExtraData.hpp"
NiFloatsExtraData::NiFloatsExtraData(NifReader& reader) : NiExtraData(reader) {
	reader.read(&numFloats, sizeof(uint32_t), 1);
	data.resize(numFloats);
	reader.read(&data[0], sizeof(float), numFloats);
}
NiObject* NiFloatsExtraData::create(NifReader& reader) {
	return new NiFloatsExtraData(reader);
}
NiFloatsExtraData::~NiFloatsExtraData() {
}
void NiFloatsExtraData::resolvePointers(NifData& data) {
	NiExtraData::resolvePointers(data);
}

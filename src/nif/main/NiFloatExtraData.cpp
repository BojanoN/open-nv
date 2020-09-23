#include "NiFloatExtraData.hpp"
NiFloatExtraData::NiFloatExtraData(NifReader& reader) : NiExtraData(reader) {
	reader.read(&floatData, sizeof(float), 1);
}
NiObject* NiFloatExtraData::create(NifReader& reader) {
	return new NiFloatExtraData(reader);
}
NiFloatExtraData::~NiFloatExtraData() {
}
void NiFloatExtraData::resolvePointers(NifData& data) {
	NiExtraData::resolvePointers(data);
}

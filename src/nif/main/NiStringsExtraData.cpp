#include "NiStringsExtraData.hpp"
NiStringsExtraData::NiStringsExtraData(NifReader& reader) : NiExtraData(reader) {
	reader.read(&numStrings, sizeof(uint32_t), 1);
	data.resize(numStrings);
	for(unsigned int i = 0; i < numStrings; i++) {
		data.push_back(reader.loadSizedString());
	}
}
NiObject* NiStringsExtraData::create(NifReader& reader) {
	return new NiStringsExtraData(reader);
}
NiStringsExtraData::~NiStringsExtraData() {
}
void NiStringsExtraData::resolvePointers(NifData& data) {
	NiExtraData::resolvePointers(data);
}

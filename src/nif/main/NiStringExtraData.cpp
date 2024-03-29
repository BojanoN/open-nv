#include "NiStringExtraData.hpp"
NiStringExtraData::NiStringExtraData(NifReader& reader) : NiExtraData(reader) {
	stringData = reader.readIndexedString();
}
NiObject* NiStringExtraData::create(NifReader& reader) {
	return new NiStringExtraData(reader);
}
NiStringExtraData::~NiStringExtraData() {
}
void NiStringExtraData::resolvePointers(NifData& data) {
	NiExtraData::resolvePointers(data);
}

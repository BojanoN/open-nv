#include "NiExtraData.hpp"
NiExtraData::NiExtraData(NifReader& reader) : NiObject(reader) {
	name = reader.readIndexedString();
}
NiObject* NiExtraData::create(NifReader& reader) {
	return new NiExtraData(reader);
}
NiExtraData::~NiExtraData() {
}
void NiExtraData::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
}

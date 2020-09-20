#include "NiAlphaProperty.hpp"
NiAlphaProperty::NiAlphaProperty(NifReader& reader) : NiProperty(reader) {
	uint16_t flagsData;
	reader.read(&flagsData, sizeof(uint16_t), 1);
	flags = flagsData;
	reader.read(&threshold, sizeof(uint8_t), 1);
}
NiObject* NiAlphaProperty::create(NifReader& reader) {
	return new NiAlphaProperty(reader);
}
NiAlphaProperty::~NiAlphaProperty() {
}
void NiAlphaProperty::resolvePointers(NifData& data) {
	NiProperty::resolvePointers(data);
}

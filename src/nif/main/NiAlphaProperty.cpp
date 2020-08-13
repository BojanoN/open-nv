#include "NiAlphaProperty.hpp"
NiAlphaProperty::NiAlphaProperty(NifReader& reader) : NiProperty(reader) {
	reader.read(&flags, sizeof(AlphaFlags), 1);
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

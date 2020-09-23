#include "NiShadeProperty.hpp"
NiShadeProperty::NiShadeProperty(NifReader& reader) : NiProperty(reader) {
	reader.read(&flags, sizeof(NiEnums::ShadeFlags), 1);
}
NiObject* NiShadeProperty::create(NifReader& reader) {
	return new NiShadeProperty(reader);
}
NiShadeProperty::~NiShadeProperty() {
}
void NiShadeProperty::resolvePointers(NifData& data) {
	NiProperty::resolvePointers(data);
}

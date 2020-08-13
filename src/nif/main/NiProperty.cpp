#include "NiProperty.hpp"
NiProperty::NiProperty(NifReader& reader) : NiObjectNET(reader) {
}
NiObject* NiProperty::create(NifReader& reader) {
	return new NiProperty(reader);
}
NiProperty::~NiProperty() {
}
void NiProperty::resolvePointers(NifData& data) {
	NiObjectNET::resolvePointers(data);
}

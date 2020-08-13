#include "NiMaterialProperty.hpp"
NiMaterialProperty::NiMaterialProperty(NifReader& reader) : NiProperty(reader) {
	specularColor = new Color3(reader);
	emissiveColor = new Color3(reader);
	reader.read(&glossiness, sizeof(float), 1);
	reader.read(&alpha, sizeof(float), 1);
	reader.read(&emissiveMult, sizeof(float), 1);
}
NiObject* NiMaterialProperty::create(NifReader& reader) {
	return new NiMaterialProperty(reader);
}
NiMaterialProperty::~NiMaterialProperty() {
	delete specularColor;
	delete emissiveColor;
}
void NiMaterialProperty::resolvePointers(NifData& data) {
	NiProperty::resolvePointers(data);
	specularColor->resolvePointers(data);
	emissiveColor->resolvePointers(data);
}

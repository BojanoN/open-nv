#include "NiMaterialProperty.hpp"
NiMaterialProperty::NiMaterialProperty(NifReader& reader) : NiProperty(reader) {
	specularColor = std::make_shared<Color3>();
	specularColor->load(reader);
	emissiveColor = std::make_shared<Color3>();
	emissiveColor->load(reader);
	reader.read(&glossiness, sizeof(float), 1);
	reader.read(&alpha, sizeof(float), 1);
	reader.read(&emissiveMult, sizeof(float), 1);
}
NiObject* NiMaterialProperty::create(NifReader& reader) {
	return new NiMaterialProperty(reader);
}
NiMaterialProperty::~NiMaterialProperty() {
}
void NiMaterialProperty::resolvePointers(NifData& data) {
	NiProperty::resolvePointers(data);
	specularColor->resolvePointers(data);
	emissiveColor->resolvePointers(data);
}

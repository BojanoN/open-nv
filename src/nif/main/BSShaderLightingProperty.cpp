#include "BSShaderLightingProperty.hpp"
BSShaderLightingProperty::BSShaderLightingProperty(NifReader& reader) : BSShaderProperty(reader) {
	reader.read(&textureClampMode, sizeof(NiEnums::TexClampMode), 1);
}
NiObject* BSShaderLightingProperty::create(NifReader& reader) {
	return new BSShaderLightingProperty(reader);
}
BSShaderLightingProperty::~BSShaderLightingProperty() {
}
void BSShaderLightingProperty::resolvePointers(NifData& data) {
	BSShaderProperty::resolvePointers(data);
}

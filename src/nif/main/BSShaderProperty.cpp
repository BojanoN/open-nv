#include "BSShaderProperty.hpp"
BSShaderProperty::BSShaderProperty(NifReader& reader) : NiShadeProperty(reader) {
	reader.read(&shaderType, sizeof(NiEnums::BSShaderType), 1);
	reader.read(&shaderFlags, sizeof(NiEnums::BSShaderFlags), 1);
	reader.read(&shaderFlags2, sizeof(NiEnums::BSShaderFlags2), 1);
	reader.read(&environmentMapScale, sizeof(float), 1);
}
NiObject* BSShaderProperty::create(NifReader& reader) {
	return new BSShaderProperty(reader);
}
BSShaderProperty::~BSShaderProperty() {
}
void BSShaderProperty::resolvePointers(NifData& data) {
	NiShadeProperty::resolvePointers(data);
}

#include "BSShaderPPLightingProperty.hpp"
BSShaderPPLightingProperty::BSShaderPPLightingProperty(NifReader& reader) : BSShaderLightingProperty(reader) {
textureSet.load(reader);
	reader.read(&refractionStrength, sizeof(float), 1);
	reader.read(&refractionFirePeriod, sizeof(int32_t), 1);
	reader.read(&parallaxMaxPasses, sizeof(float), 1);
	reader.read(&parallaxScale, sizeof(float), 1);
}
NiObject* BSShaderPPLightingProperty::create(NifReader& reader) {
	return new BSShaderPPLightingProperty(reader);
}
BSShaderPPLightingProperty::~BSShaderPPLightingProperty() {
}
void BSShaderPPLightingProperty::resolvePointers(NifData& data) {
	BSShaderLightingProperty::resolvePointers(data);
this->textureSet.resolve(data);
}

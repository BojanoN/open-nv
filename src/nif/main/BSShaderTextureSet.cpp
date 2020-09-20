#include "BSShaderTextureSet.hpp"
BSShaderTextureSet::BSShaderTextureSet(NifReader& reader) : NiObject(reader) {
	reader.read(&numTextures, sizeof(uint32_t), 1);
	textures.resize(numTextures);
	for(unsigned int i = 0; i < numTextures; i++) {
		textures.push_back(reader.loadSizedString());
	}
}
NiObject* BSShaderTextureSet::create(NifReader& reader) {
	return new BSShaderTextureSet(reader);
}
BSShaderTextureSet::~BSShaderTextureSet() {
}
void BSShaderTextureSet::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
}

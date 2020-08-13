#include "BSShaderTextureSet.hpp"
BSShaderTextureSet::BSShaderTextureSet(NifReader& reader) : NiObject(reader) {
	reader.read(&numTextures, sizeof(uint32_t), 1);
	textures = new char*[numTextures];
	for(unsigned int i = 0; i < numTextures; i++) {
		textures[i] = reader.loadSizedString();
	}
}
NiObject* BSShaderTextureSet::create(NifReader& reader) {
	return new BSShaderTextureSet(reader);
}
BSShaderTextureSet::~BSShaderTextureSet() {
	for(unsigned int i = 0; i < numTextures; i++) {
		delete textures[i];
	}
	delete[] textures;
}
void BSShaderTextureSet::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
}

#include "texture_manager.hpp"

Texture2D* TextureManager::getTexture(const std::string& texturePath) {

	auto[cachedTexture, ok] = textureCache.getCachedObject(texturePath);
	if(ok) {
		log_debug("Found %s in cache", texturePath.c_str());
		return cachedTexture;
	}

	log_debug("Reading %s from file", texturePath.c_str());
	std::vector<uint8_t> textureFile = getFileProvider().readFile(texturePath);

	log_debug("Read file of size %d", textureFile.size());

	uint32_t magic = *(reinterpret_cast<uint32_t*>(&textureFile[0]));
	
	if (magic != ddsMagic) {
		log_error("Magic byte wrong!\n");
		throw std::runtime_error("Cannot read texture");
	}
	ddsHeader header = *(reinterpret_cast<ddsHeader*>(&textureFile[4]));
	
	log_debug("DDS header flags:  0x%08x", header.flags);
	log_debug("DDS header caps 1: 0x%08x", header.caps);
	log_debug("DDS header caps 2: 0x%08x", header.caps2);
	log_debug("DDS header pixel format flags: 0x%08x", header.pixelFormat.flags);
	log_debug("DDS four cc: 0x%08x", header.pixelFormat.fourcc);

	std::vector<uint8_t> textureData(textureFile.begin() + 128, textureFile.end());

	log_debug("Texture data has size %d", textureData.size());

	Texture2D* texture = new Texture2D(header, textureData);
	
	textureCache.insert(texturePath, texture);
	return texture;
}
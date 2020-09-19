#include "texture2D.hpp"

Texture2D::Texture2D(const std::string& texturePath) {

	std::vector<uint8_t> textureFile = getFileProvider().readFile(texturePath);

	uint32_t magic = *(reinterpret_cast<uint32_t*>(&textureFile[0]));
	
	if (magic != DDS::ddsMagic) {
		log_error("Magic byte wrong!\n");
		throw std::runtime_error("Cannot read texture");
	}
	this->header = *(reinterpret_cast<DDS::ddsHeader*>(&textureFile[4]));
	
	log_debug("DDS header flags:  0x%08x", header.flags);
	log_debug("DDS header caps 1: 0x%08x", header.caps);
	log_debug("DDS header caps 2: 0x%08x", header.caps2);
	log_debug("DDS header pixel format flags: 0x%08x", header.pixelFormat.flags);
	log_debug("DDS four cc: 0x%08x", header.pixelFormat.fourcc);

	this->textureData.assign(textureFile.begin() + DDS::ddsDataOffset, textureFile.end());
}
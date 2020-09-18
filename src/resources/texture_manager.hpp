#pragma once
#include "texture2D.hpp"
#include "../file/reader.hpp"
#include "resource_cache.hpp"
#include <string>
#include <memory>


class TextureManager : public File::Reader {

private:

	ResourceCache<std::string, Texture2D> textureCache;

public:

	TextureManager() {}

	std::shared_ptr<Texture2D> getTexture(const std::string& texturePath);
};
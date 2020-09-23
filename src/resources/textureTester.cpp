#include "texture2D.hpp"

int main(int argc, char const *argv[])
{
	/*
	TextureManager texMan;
	{
		std::shared_ptr<Texture2D> texture = texMan.getTexture("textures\\clutter\\food\\BeerBottle.dds");
		std::cout << "Use count: " << texture.use_count() << std::endl;
		{
			std::shared_ptr<Texture2D> sameTexture = texMan.getTexture("textures\\clutter\\food\\BeerBottle.dds");
			std::cout << "Use count: " << texture.use_count() << std::endl;
		}
		std::cout << "Use count: " << texture.use_count() << std::endl;
		std::cout << "Cache size: " << texMan.cacheSize() << std::endl;
	}
	std::cout << "Should drop from cache now" << std::endl;
	std::cout << "Cache size: " << texMan.cacheSize() << std::endl;
	{
		std::shared_ptr<Texture2D> texture = texMan.getTexture("textures\\clutter\\food\\BeerBottle.dds");
	}*/
	{
		std::shared_ptr<Texture2D> texture = Texture2D::get("textures\\clutter\\food\\BeerBottle.dds");
		std::cout << "Use count: " << texture.use_count() << std::endl;
		{
			std::shared_ptr<Texture2D> same = Texture2D::get("textures\\clutter\\food\\BeerBottle.dds");
			std::cout << "Use count: " << texture.use_count() << std::endl;
		}
	}
	std::cout << "Should drop from cache now" << std::endl;
	{
		std::shared_ptr<Texture2D> texture = Texture2D::get("textures\\clutter\\food\\BeerBottle.dds");
		std::cout << "Use count: " << texture.use_count() << std::endl;
	}
	std::cout << "Cache size: " << Texture2D::cacheSize() << std::endl;

	Texture2D::destroyFileProvider();

	return 0;
}
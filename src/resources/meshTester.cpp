#include "nif/nifdata.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	/*
	MeshManager meshMan;
	
	{
		std::shared_ptr<NifData> mesh = meshMan.getMesh("AnimObjects\\aoBeerBottle.NIF");
		std::cout << "Use count: " << mesh.use_count() << std::endl;
		{
			std::shared_ptr<NifData> mesh = meshMan.getMesh("AnimObjects\\aoBeerBottle.NIF");
			std::cout << "Use count: " << mesh.use_count() << std::endl;			
		}
		std::cout << "Use count: " << mesh.use_count() << std::endl;
	}
	std::shared_ptr<NifData> mesh = meshMan.getMesh("AnimObjects\\aoBeerBottle.NIF");
	std::cout << "Use count: " << mesh.use_count() << std::endl;
*/
	{
		std::shared_ptr<NifData> mesh = NifData::get("AnimObjects\\aoBeerBottle.NIF");
		std::cout << "Use count: " << mesh.use_count() << std::endl;
		{
			std::shared_ptr<NifData> same = NifData::get("AnimObjects\\aoBeerBottle.NIF");
			std::cout << "Use count: " << mesh.use_count() << std::endl;
		}
	}
	std::cout << "Should drop from cache now" << std::endl;
	{
		std::shared_ptr<NifData> mesh = NifData::get("AnimObjects\\aoBeerBottle.NIF");
		std::cout << "Use count: " << mesh.use_count() << std::endl;
	}
	std::cout << "Cache size: " << NifData::cacheSize() << std::endl;

	NifData::destroyFileProvider();
	return 0;
}
#include "mesh_manager.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
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

	return 0;
}
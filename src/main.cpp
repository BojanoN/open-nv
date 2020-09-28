#include "engine.hpp"
#include <string>
#include <getopt.h>


int main(int argc, char** argv)
{
	using Engine::Engine;

	std::string installPath;
	std::string openNVConfigPath = ".";
		
	int c;
	while ((c = getopt(argc, argv, "i:")) != -1) {
        switch (c) {
        case 'i':
            installPath = optarg;
            break;
        case 'c':
        	openNVConfigPath = optarg;
        	break;
        default:
            break;
        }
    }

    Engine engine(openNVConfigPath, installPath);
    engine.start();

	return 0;
}
#include "configuration_manager.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char const *argv[])
{
	File::ConfigurationManager manager;
	manager.loadFile("/media/crop/06A0AC67A0AC5F45/GOG Games/Fallout New Vegas/Fallout_default.ini");

	File::Configuration& config = manager.getConfiguration("General");

	std::cout << config.getString("sEssentialFileCacheList") << std::endl;
	std::cout << config.getDouble("fStaticScreenWaitTime") << std::endl;
	std::cout << config.getInt("iHoursToSleep") << std::endl;
	std::cout << config.getBool("bAnimationUseBlendFromPose") << std::endl;

	return 0;
}
#pragma once

#include <file/configuration_manager.hpp>
#include <gameworld.hpp>
#include <esm/reader.hpp>

#include <filesystem>
#include <string>


namespace Engine {


using File::ConfigurationManager;
using GameWorld::GameWorld;

namespace fs = std::filesystem;

class Engine {

private:

	static inline const std::string dataFolderName = "Data";

	ConfigurationManager configManager;
	fs::path dataPath;
	GameWorld world;

public:

	Engine(const std::string& configPath, const std::string& installPath);
	~Engine();

	void start() {}

};

}; // namespace Engine
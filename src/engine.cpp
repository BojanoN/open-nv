#include "engine.hpp"
#include <vector>
#include <algorithm>

namespace Engine {


namespace fs = std::filesystem;

Engine::Engine(const std::string& configPath, const std::string& installPath) {

	for(auto& file : fs::directory_iterator(configPath)) {
		if(file.path().extension() == ".ini") {
			configManager.loadFile(file.path().string());
		}
	}

	this->dataPath = fs::path(installPath) / dataFolderName;

	File::Reader::setRootFilePath(this->dataPath);

	std::vector<fs::directory_entry> mastersPlugins;

	for(auto& file : fs::directory_iterator(dataPath)) {
		if(file.path().extension() == ".esm" || file.path().extension() == ".esp") {
			mastersPlugins.push_back(file);
		}
	}

	world.loadMastersAndPlugins(mastersPlugins);
}

Engine::~Engine() {
	File::Reader::destroyFileProvider();
}

}; // namespace Engine
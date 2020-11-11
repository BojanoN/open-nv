#pragma once

#include <esm/reader.hpp>
#include <file/configuration_manager.hpp>
#include <gameworld.hpp>
#include <resources/shader.hpp>

#include <filesystem>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace Engine {

using File::ConfigurationManager;
using GameWorld::GameWorld;

namespace fs = std::filesystem;

class Engine {

private:
    static inline const std::string dataFolderName = "Data";

    ConfigurationManager configManager;

    fs::path workingDirectory;
    fs::path dataPath;
    fs::path resourcePath;

    std::vector<fs::path> mastersPlugins;
    GameWorld             world;

    SDL_Window*   window;
    SDL_GLContext openGLContext;

    bool initSDL();
    bool loadConfigurationFiles();
    bool loadGameSettings();

public:
    Engine(const std::string& configPath, const std::string& installPath);
    ~Engine();

    bool start();
};

}; // namespace Engine

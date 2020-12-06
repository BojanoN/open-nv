#pragma once

#include <esm/reader.hpp>
#include <file/configuration_manager.hpp>
#include <game/fnv/fnv.hpp>
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
    static inline const std::string dirNameGameData                  = "Data";
    static inline const char*       envHome                          = "HOME";
    static inline const std::string dirNameShader                    = "shader";
    static inline const std::string dirNameResource                  = "res";
    static inline const std::string dirNameUserConfiguration         = ".opennv";
    static inline const std::string fileNameGameDefaultConfiguration = "Fallout_default.ini";
    static inline const std::string dirNameVideo                     = "Video";
    static inline const std::string fileNameIntroMovie               = "FNVIntro.bik";
    static inline const std::string extConfigurationFile             = ".ini";
    static inline const std::string extMasterFile                    = ".esm";
    static inline const std::string extPluginFile                    = ".esp";

    static inline const char* displayConfigurationName = "Display";
    static inline const char* cfgScreenWidth           = "uSize W";
    static inline const char* cfgScreenHeight          = "uSize H";
    static inline const char* windowName               = "OpenNV";

    ConfigurationManager configManager;

    fs::path dirInstall;
    fs::path dirWorking;
    fs::path dirData;
    fs::path dirResource;

    std::vector<fs::path> mastersPlugins;
    GameWorld             world;
    FalloutNVGame         game;

    SDL_Window*   window;
    SDL_GLContext openGLContext;

    bool initSDL();
    bool initConfiguration();
    bool loadConfigurationFiles();
    bool loadGameSettings();

public:
    Engine(const std::string& configPath, const std::string& dirInstall);
    ~Engine();

    bool start();
};

}; // namespace Engine

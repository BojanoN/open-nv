#include "engine.hpp"

#include <engine/audio.hpp>
#include <engine/video.hpp>
#include <logc/log.h>

#include <algorithm>
#include <stdexcept>
#include <vector>

#include <cstdlib>

namespace Engine {

namespace fs = std::filesystem;

Engine::Engine(const std::string& configPath, const std::string& installPath)
{

    this->workingDirectory = fs::current_path();
    this->resourcePath     = this->workingDirectory / "res";
    this->dataPath         = fs::path(installPath) / dataFolderName;

    File::Reader::setRootFilePath(this->dataPath);
    ShaderManager::shaderPath = this->resourcePath / "shader";

    // Probe user config directory
    // Load all configuration present in user directory
    const char* userHomeDir = std::getenv("HOME");
    if (!userHomeDir) {
        throw std::runtime_error("Unable to get user home directory!");
    }

    fs::path userHomeDirPath { userHomeDir };

    // Check if user config directory exists
    fs::path        userConfigurationDirectory       = userHomeDirPath / ".opennv";
    fs::file_status userConfigurationDirectoryStatus = fs::status(userConfigurationDirectory);

    if (!fs::exists(userConfigurationDirectoryStatus)) {
        // Create and copy ini files to user directory
        log_info("User configuration directory not found at %s, creating...", userConfigurationDirectory.string().c_str());
        bool created = fs::create_directory(userConfigurationDirectory);

        if (!created) {
            throw std::runtime_error("Unable to create user configuration directory!");
        }

        fs::copy(dataPath / "Fallout_default.ini", userConfigurationDirectory / "Fallout.ini");

        log_info("Loading configuration files from install directory...");
        for (auto& file : fs::directory_iterator(configPath)) {
            if (file.path().extension() == ".ini") {
                configManager.loadFile(file.path().string());
            }
        }

    } else {
        log_info("Loading configuration files from %s ...", userConfigurationDirectory.string().c_str());
        for (auto& file : fs::directory_iterator(userConfigurationDirectory)) {
            if (file.path().extension() == ".ini") {
                configManager.loadFile(file.path().string());
            }
        }
    }

    for (auto& file : fs::directory_iterator(dataPath)) {
        if (file.path().extension() == ".esm" || file.path().extension() == ".esp") {
            this->mastersPlugins.push_back(file.path());
        }
    }
}

Engine::~Engine()
{
    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(this->window);
    SDL_Quit();

    AudioEngine::close();

    File::Reader::destroyFileProvider();
}

bool Engine::initSDL()
{

    // TODO: load resolution from settings

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    size_t windowHeight, windowWidth;

    File::Configuration& displayConfiguration = this->configManager.getConfiguration("Display");

    // Check for previously defined resolution
    try {

        windowWidth  = displayConfiguration.getUInt("iSize W");
        windowHeight = displayConfiguration.getUInt("iSize H");

    } catch (std::runtime_error& e) {
        // Fallback to native is none
        log_info("No previous screen size defined, using native display size...");

        SDL_DisplayMode currentDisplayMode;
        // Support only one display for now
        int err = SDL_GetCurrentDisplayMode(0, &currentDisplayMode);

        if (err != 0) {
            log_error("SDL init error: %s", SDL_GetError());
            return false;
        }

        windowWidth  = currentDisplayMode.w;
        windowHeight = currentDisplayMode.h;

        displayConfiguration.setUInt("iSize W", windowWidth);
        displayConfiguration.setUInt("iSize H", windowHeight);
    }

    this->window = SDL_CreateWindow("OpenNV", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        return false;
    }

    this->openGLContext = SDL_GL_CreateContext(this->window);

    glewInit();

    return true;
}

bool Engine::start()
{

    File::Configuration& displayConfiguration = this->configManager.getConfiguration("Display");

    if (!AudioEngine::init()) {
        log_fatal("Audio engine initialization failed!");

        return false;
    }

    try {
        if (!initSDL()) {
            log_fatal("SDL initialization failed!");

            return false;
        }
    } catch (std::runtime_error& e) {
        log_fatal(e.what());
        return false;
    }

    unsigned int displayWidth  = displayConfiguration.getUInt("iSize W");
    unsigned int displayHeight = displayConfiguration.getUInt("iSize H");

    fs::path introMoviePath = this->dataPath / "Video/FNVIntro.bik";

    VideoPlayer* introMoviePlayer = new VideoPlayer(displayWidth, displayHeight);
    if (introMoviePlayer->play(introMoviePath.string().c_str(), this->window) < 0) {
        log_error("Failed to play intro video!");
    }

    try {
        world.loadMastersAndPlugins(this->mastersPlugins);
    } catch (std::runtime_error& e) {
        return false;
    }

    // Load game settings
    for (fs::path& gameFile : mastersPlugins) {
        world.loadGameSettings(gameFile, this->configManager);
    }

    return true;
}

}; // namespace Engine

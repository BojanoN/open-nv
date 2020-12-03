#include "engine.hpp"

#include <engine/audio.hpp>
#include <engine/video.hpp>
#include <logc/log.h>

#include <algorithm>
#include <stdexcept>
#include <vector>

#include <cstdlib>
#include <system_error>


namespace Engine {

namespace fs = std::filesystem;

Engine::Engine(const std::string& configPath, const std::string& dirInstall)
    : game(this->world)
{
    this->dirInstall = fs::path(dirInstall);
    this->dirWorking = fs::current_path();
    this->dirResource     = this->dirWorking / dirNameResource;
    this->dirData         = this->dirInstall / dirNameGameData;
    File::Reader::setRootFilePath(this->dirData);
}


Engine::~Engine()
{
    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(this->window);
    SDL_Quit();

    AudioEngine::close();

    File::Reader::destroyFileProvider();
}


bool Engine::initConfiguration() {
    ShaderManager::shaderPath = this->dirResource / dirNameShader;

    // Probe user config directory
    // Load all configuration present in user directory
    const char* envvalHome = std::getenv(envHome);
    if (envvalHome == NULL) {
        log_error("Unable to get user home directory!");
        return false;
    }

    fs::path dirUserHome { envvalHome };

    // Check if user config directory exists
    fs::path        dirUserConfiguration       = dirUserHome / dirNameUserConfiguration;
    fs::file_status statusDirUserConfiguration = fs::status(dirUserConfiguration);

    if (!fs::exists(statusDirUserConfiguration)) {
        // Create and copy ini files to user directory
        log_info("User configuration directory not found at %s, creating...", dirUserConfiguration.string().c_str());
        bool createdDirUserConfiguration = fs::create_directory(dirUserConfiguration);

        if (!createdDirUserConfiguration) {
            log_error("Unable to create user configuration directory!");
            return false;
        }

        std::error_code errorInCopy;
        fs::copy(fs::path(this->dirInstall) / fileNameGameDefaultConfiguration, dirUserConfiguration / fileNameGameDefaultConfiguration, errorInCopy);

        if(errorInCopy) {
            log_error("Unable to copy configuration file.");
            return false;
        }

    }

    log_info("Loading configuration files from %s ...", dirUserConfiguration.string().c_str());
 
    for (auto& file : fs::directory_iterator(dirUserConfiguration)) {
        
        if (file.path().extension() == extConfigurationFile) {
            log_info("Loading configuration file: %s ...", file.path().string().c_str());
            
            bool loadedConfigFromFile = configManager.loadFile(file.path().string());
            if(!loadedConfigFromFile) {
                log_warn("Error while reading configuration from file %s", file.path().string().c_str());
            }
        }
    }

    log_info("Configuration files loaded.");
    log_info("Finding master and plugin files...");

    for (auto& file : fs::directory_iterator(this->dirData)) {
        if (file.path().extension() == extMasterFile || file.path().extension() == extPluginFile) {
            this->mastersPlugins.push_back(file.path());
        }
    }
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

    File::Configuration& displayConfiguration = this->configManager.getConfiguration(displayConfigurationName);

    // Check for previously defined resolution
    try {

        windowWidth  = displayConfiguration.getUInt(cfgScreenWidth);
        windowHeight = displayConfiguration.getUInt(cfgScreenHeight);

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

        displayConfiguration.setUInt(cfgScreenWidth, windowWidth);
        displayConfiguration.setUInt(cfgScreenHeight, windowHeight);
    }

    this->window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        return false;
    }

    this->openGLContext = SDL_GL_CreateContext(this->window);

    glewInit();

    return true;
}

bool Engine::start()
{
    log_info("Loading configuration...");
    
    bool initializedConfiguration = initConfiguration();
    if(!initializedConfiguration) {
        
        log_fatal("Cannot initialize configuration.");
        return false;
    }

    log_info("Initializing engine components.");

    File::Configuration& displayConfiguration = this->configManager.getConfiguration(displayConfigurationName);

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

    unsigned int displayWidth  = displayConfiguration.getUInt(cfgScreenWidth);
    unsigned int displayHeight = displayConfiguration.getUInt(cfgScreenHeight);

    fs::path fileIntroMovie = (this->dirData / dirNameVideo) / fileNameIntroMovie;

    VideoPlayer* introMoviePlayer = new VideoPlayer(displayWidth, displayHeight);
    if (introMoviePlayer->play(fileIntroMovie.string().c_str(), this->window) < 0) {
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

    game.load();

    return true;
}

}; // namespace Engine

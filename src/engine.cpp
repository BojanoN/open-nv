#include "engine.hpp"

#include <engine/audio.hpp>
#include <engine/video.hpp>
#include <logc/log.h>

#include <algorithm>
#include <vector>

namespace Engine {

namespace fs = std::filesystem;

Engine::Engine(const std::string& configPath, const std::string& installPath)
{

    this->workingDirectory = fs::current_path();

    this->resourcePath = this->workingDirectory;
    this->resourcePath += "/res/";

    for (auto& file : fs::directory_iterator(configPath)) {
        if (file.path().extension() == ".ini") {
            configManager.loadFile(file.path().string());
        }
    }

    this->dataPath = fs::path(installPath) / dataFolderName;

    File::Reader::setRootFilePath(this->dataPath);

    for (auto& file : fs::directory_iterator(dataPath)) {
        if (file.path().extension() == ".esm" || file.path().extension() == ".esp") {
            this->mastersPlugins.push_back(file);
        }
    }

    ShaderManager::shaderPath = this->resourcePath;
    ShaderManager::shaderPath += "/shader/";
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

    this->window = SDL_CreateWindow("OpenNV", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        return false;
    }

    this->openGLContext = SDL_GL_CreateContext(this->window);

    glewInit();

    return true;
}

bool Engine::start()
{

    if (!AudioEngine::init()) {
        log_fatal("Audio engine initialization failed!");

        return false;
    }

    if (!initSDL()) {
        log_fatal("SDL initialization failed!");

        return false;
    }

    fs::path introMoviePath = this->dataPath;
    introMoviePath += "/Video/FNVIntro.bik";

    VideoPlayer* introMoviePlayer = new VideoPlayer();
    if (introMoviePlayer->play(introMoviePath.string().c_str(), this->window) < 0) {
        log_error("Failed to play intro video!");
    }

    try {
        world.loadMastersAndPlugins(this->mastersPlugins);
    } catch (std::runtime_error& e) {
        return false;
    }

    return true;
}

}; // namespace Engine

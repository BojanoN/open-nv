#pragma once

#include <error/error.hpp>
#include <gameworld.hpp>

using File::ConfigurationManager;

struct SDL_Window;

namespace Game {
namespace fs = std::filesystem;

class Game {
public:
    Game(GameWorld::GameWorld& w, const std::string& dirInstall)
        : world(w)
        , dirInstallDir(dirInstall) {};

    virtual Err::Error start()                       = 0;
    virtual void       setWindow(SDL_Window* window) = 0;

protected:
    GameWorld::GameWorld& world;
    fs::path              dirInstallDir;
};

}

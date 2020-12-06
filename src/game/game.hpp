#pragma once

#include <gameworld.hpp>

namespace Game {

class Game {
public:
    Game(GameWorld::GameWorld& w)
        : world(w) {};

    virtual void load() = 0;

protected:
    GameWorld::GameWorld& world;
};

}

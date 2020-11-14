#pragma once

#include <gameworld.hpp>

class Game {
public:
    Game(GameWorld::GameWorld& w)
        : world(w) {};

    virtual void load() = 0;

protected:
    GameWorld::GameWorld& world;
};

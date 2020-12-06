#pragma once

#include "../game.hpp"

#include <esm/types.hpp>
#include <unordered_set>

class FalloutNVGame : public Game::Game {
public:
    FalloutNVGame(GameWorld::GameWorld& gw)
        : Game::Game(gw)
    {
    }
    void load();

private:
    static const std::unordered_set<ESM::ESMType> initialToplevelGroups;
};

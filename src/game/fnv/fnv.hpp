#pragma once

#include "../game.hpp"
#include "../rendermanager.hpp"

#include <esm/types.hpp>
#include <unordered_set>

namespace Game {

class FalloutNVGame : public Game::Game {
public:
    FalloutNVGame(GameWorld::GameWorld& gw)
        : Game(gw)
    {
    }
    void start();

private:
    static const std::unordered_set<ESM::ESMType> initialToplevelGroups;

    RenderStateManager mRenderManager;
};
}

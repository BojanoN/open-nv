#pragma once

#include "../game.hpp"
#include "../rendermanager.hpp"

#include <esm/types.hpp>
#include <unordered_set>

namespace Game {

class FalloutNVGame : public Game::Game {
public:
    FalloutNVGame(GameWorld::GameWorld& gw, const std::string& dirInstall)
        : Game(gw, dirInstall)
    {
    }

    virtual Err::Error start();
    virtual void       setWindow(SDL_Window* window);

private:
    static const std::unordered_set<ESM::ESMType> initialToplevelGroups;

    RenderStateManager mRenderManager;
};
}

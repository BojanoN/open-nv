#pragma once

#include <vector>

#include "statemanager.hpp"

struct SDL_Window;

namespace Game {

class RenderStateManager : protected GameStateManager {
public:
    RenderStateManager() {};

    void                        push(std::shared_ptr<GameState>& state);
    std::shared_ptr<GameState>  pop();
    std::shared_ptr<GameState>& peek();

    Err::Error enter();
    void       exit();

    bool draw();

    SDL_Window* mSDLWindow = nullptr;
};

}

#pragma once

#include <vector>

#include "statemanager.hpp"

namespace Game {

class RenderStateManager : protected GameStateManager {
public:
    void                        push(std::shared_ptr<GameState>& state);
    std::shared_ptr<GameState>  pop();
    std::shared_ptr<GameState>& peek();

    void draw();

private:
    // std::vector<DrawableGameState> mActiveDrawableStates;
};

}

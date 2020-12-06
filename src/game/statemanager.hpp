#pragma once

#include <memory>
#include <stack>

#include "state.hpp"

namespace Game {

class GameStateManager {
public:
    virtual void                        push(std::shared_ptr<GameState>& state) = 0;
    virtual std::shared_ptr<GameState>  pop()                                   = 0;
    virtual std::shared_ptr<GameState>& peek()                                  = 0;

protected:
    std::stack<std::shared_ptr<GameState>> mStateStack;
};

}

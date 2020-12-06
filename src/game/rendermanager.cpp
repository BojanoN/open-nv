#include "rendermanager.hpp"

namespace Game {

void RenderStateManager::push(std::shared_ptr<GameState>& state)
{
    if (state->mStateType != GameStateType::Drawable) {
        return;
    }
}

std::shared_ptr<GameState> RenderStateManager::pop()
{
    std::shared_ptr<GameState> ret = mStateStack.top();

    mStateStack.pop();

    return ret;
}

std::shared_ptr<GameState>& RenderStateManager::peek()
{
    return mStateStack.top();
}

void RenderStateManager::draw()
{
    /* size_t szActiveDrawablesSize = mActiveDrawableStates.size();

    for (size_t i = 0; i < szActiveDrawablesSize; i++) {
        mActiveDrawableStates[i]->draw();
    }
  */
    static_cast<DrawableGameState*>(mStateStack.top().get())->draw();
}

}

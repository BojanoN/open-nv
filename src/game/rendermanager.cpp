#include "rendermanager.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>

namespace Game {

void RenderStateManager::push(std::shared_ptr<GameState>& state)
{
    if (state->mStateType != GameStateType::Drawable) {
        return;
    }

    mStateStack.push(std::move(state));
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

Err::Error RenderStateManager::enter()
{
    return (mStateStack.top())->enter();
}

void RenderStateManager::exit()
{
    return (mStateStack.top())->exit();
}

bool RenderStateManager::draw()
{
    /* size_t szActiveDrawablesSize = mActiveDrawableStates.size();

    for (size_t i = 0; i < szActiveDrawablesSize; i++) {
        mActiveDrawableStates[i]->draw();
    }
  */
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                SDL_GL_SwapWindow(mSDLWindow);

                return false;
            }
        }
    }

    DrawableGameStateStatus statusDraw = static_cast<DrawableGameState*>(mStateStack.top().get())->draw();
    if (statusDraw == DrawableGameStateStatus::EndOfOperation) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return false;
    }
    if (statusDraw == DrawableGameStateStatus::ScreenContentChanged) {
        SDL_GL_SwapWindow(mSDLWindow);
    }

    return true;
}

}

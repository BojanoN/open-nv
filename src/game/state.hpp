#pragma once

#include <error/error.h>

using namespace Types;

namespace Game {

enum class GameStateType {
    Dummy,
    Updateable,
    Drawable
};

class GameState {
public:
    virtual Error enter()   = 0;
    virtual void  exit()    = 0;
    virtual void  suspend() = 0;
    virtual void  resume()  = 0;

    const GameStateType mStateType;

protected:
    GameState(GameStateType type)
        : mStateType(type) {};
};

class DrawableGameState : public GameState {
public:
    DrawableGameState()
        : GameState(GameStateType::Drawable) {};

    virtual bool draw() = 0;
};

}

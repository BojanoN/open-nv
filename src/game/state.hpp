#pragma once

#include <error/error.hpp>

namespace Game {

enum class GameStateType {
    Dummy,
    Updateable,
    Drawable
};

class GameState {
public:
    virtual Err::Error enter()   = 0;
    virtual void       exit()    = 0;
    virtual void       suspend() = 0;
    virtual void       resume()  = 0;

    const GameStateType mStateType;

protected:
    GameState(GameStateType type)
        : mStateType(type) {};
};

enum class DrawableStatus {
    Continue,
    EndOfOperation,
    ScreenContentChanged,
    Error,
};

class DrawableGameState : public GameState {
public:
    DrawableGameState()
        : GameState(GameStateType::Drawable) {};

    virtual DrawableStatus draw() = 0;
};

}

#pragma once

#include <game/state.hpp>
#include <memory>
#include <vector>

class Texture2D;
class Shader;

class LoadingScreen : public Game::DrawableGameState {
public:
    virtual void enter();
    virtual void exit();
    virtual void suspend();
    virtual void resume();

    virtual void draw();

    LoadingScreen();

private:
    std::shared_ptr<Shader> mShader;

    std::vector<std::shared_ptr<Texture2D>> mLoadingScreenTextures;
};

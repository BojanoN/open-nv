#pragma once

#include <game/state.hpp>
#include <types/errorpair.hpp>

#include <memory>
#include <vector>

class Texture2D;
class Shader;

using namespace Types;

class LoadingScreen : public Game::DrawableGameState {
public:
    virtual void enter();
    virtual void exit();
    virtual void suspend();
    virtual void resume();

    virtual void draw();

    static ErrorPair<std::shared_ptr<LoadingScreen>> create();

    virtual ~LoadingScreen() {};

private:
    LoadingScreen() {};

    std::shared_ptr<Shader> mShader;

    std::vector<std::shared_ptr<Texture2D>> mLoadingScreenTextures;
};

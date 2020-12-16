#pragma once

#include <game/state.hpp>
#include <types/errorpair.hpp>

#include <resources/shader.hpp>

#include <GL/gl.h>
#include <memory>
#include <vector>

class Texture2D;
class Shader;

using namespace Types;

namespace Game {

class LoadingScreen : public DrawableGameState {
public:
    virtual Err::Error enter();
    virtual void       exit();
    virtual void       suspend();
    virtual void       resume();

    virtual DrawableStatus draw();

    static ErrorPair<std::shared_ptr<GameState>> create();

    virtual ~LoadingScreen() {};

private:
    LoadingScreen() {};

    std::shared_ptr<Shader>                 mShader;
    std::vector<std::shared_ptr<Texture2D>> mLoadingScreenTextures;

    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
    GLuint mCurrentTexture;
    GLuint mNextTexture;

    static float        vertices[16];
    static unsigned int elements[6];
};
}

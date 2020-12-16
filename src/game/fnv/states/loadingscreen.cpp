#include "loadingscreen.hpp"

#include "../constants.hpp"
#include <logc/log.h>

#include <resources/texture2D.hpp>

namespace Game {
ErrorPair<std::shared_ptr<GameState>> LoadingScreen::create()
{
    ErrorPair<std::shared_ptr<GameState>> ret;
    LoadingScreen*                        instance = new LoadingScreen();

    instance->mLoadingScreenTextures.resize(Constants::FNV::noLoadingScreenTextures);

    for (unsigned int i = 0; i < Constants::FNV::noLoadingScreenTextures; i++) {
        ErrorPair<std::shared_ptr<Texture2D>> errTexture = Texture2D::get(Constants::FNV::pathLoadingScreenTextures[i]);

        if (errTexture.fail()) {
            log_error("Unable to load texture %s", Constants::FNV::pathLoadingScreenTextures[i]);
            ret.error = errTexture.error;

            delete instance;
            return ret;
        }

        instance->mLoadingScreenTextures[i] = errTexture.value;
    }

    ErrorPair<std::shared_ptr<Shader>> errShader = ShaderManager::getShader(Constants::FNV::vsLoadingScreen, Constants::FNV::fsLoadingScreen);

    if (errShader.fail()) {
        log_error("Unable to load loading screen shaders!");
        ret.error = errShader.error;

        delete instance;
        return ret;
    }

    instance->mShader = errShader.value;

    ret.error = Err::Success;
    ret.value = std::shared_ptr<GameState>(instance);

    return ret;
}

Err::Error LoadingScreen::enter()
{
    glEnable(GL_TEXTURE_2D);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LoadingScreen::vertices), LoadingScreen::vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(LoadingScreen::elements), LoadingScreen::elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &mCurrentTexture);
    glGenTextures(1, &mNextTexture);

    return Err::Success;
}
void LoadingScreen::exit()
{
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);

    glDeleteTextures(1, &mCurrentTexture);
    glDeleteTextures(1, &mNextTexture);
}
void LoadingScreen::suspend() { }
void LoadingScreen::resume() { }

DrawableStatus LoadingScreen::draw()
{
    return DrawableStatus::EndOfOperation;
}

float LoadingScreen::vertices[16] = {
    -1.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f
};

unsigned int LoadingScreen::elements[6] = {
    0, 1, 2,
    0, 2, 3
};

}

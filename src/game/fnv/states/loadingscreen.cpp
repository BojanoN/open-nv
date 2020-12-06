#include "loadingscreen.hpp"

#include "../constants.hpp"
#include <logc/log.h>

#include <resources/shader.hpp>
#include <resources/texture2D.hpp>

ErrorPair<std::shared_ptr<LoadingScreen>> LoadingScreen::create()
{
    ErrorPair<std::shared_ptr<LoadingScreen>> ret;
    LoadingScreen*                            instance = new LoadingScreen();

    instance->mLoadingScreenTextures.resize(Constants::noLoadingScreenTextures);

    for (unsigned int i = 0; i < Constants::noLoadingScreenTextures; i++) {
        ErrorPair<std::shared_ptr<Texture2D>> errTexture = Texture2D::get(Constants::pathLoadingScreenTextures[i]);

        if (errTexture.fail()) {
            log_error("Unable to load texture %s", Constants::pathLoadingScreenTextures[i]);
            ret.error = errTexture.error;

            delete instance;
            return ret;
        }

        instance->mLoadingScreenTextures[i] = errTexture.value;
    }

    ErrorPair<std::shared_ptr<Shader>> errShader = ShaderManager::getShader(Constants::vsLoadingScreen, Constants::fsLoadingScreen);

    if (errShader.fail()) {
        log_error("Unable to load loading screen shaders!");
        ret.error = errShader.error;

        delete instance;
        return ret;
    }

    instance->mShader = errShader.value;

    ret.error
        = Err::Success;
    ret.value = std::shared_ptr<LoadingScreen>(instance);

    return ret;
}

void LoadingScreen::enter() { }
void LoadingScreen::exit() { }
void LoadingScreen::suspend() { }
void LoadingScreen::resume() { }

void LoadingScreen::draw() { }

#include "video.hpp"
#include "audio.hpp"

#include <logc/log.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

int main(int argc, char** argv)
{

    if (argc != 2) {
        log_error("Only one file needed");
        return -1;
    }

    AudioEngine::init();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window*   window  = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewInit();
    VideoPlayer* player = new VideoPlayer();

    if (player->open(argv[1]) < 0) {
        log_error("Unable to open file!");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        AudioEngine::close();

        delete player;
        return -1;
    }

    while (true) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_GL_DeleteContext(context);
                    SDL_DestroyWindow(window);
                    SDL_Quit();

                    player->close();
                    AudioEngine::close();

                    delete player;

                    return 0;
                }
            }
        }
        if (!player->update()) {
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();

            player->close();

            AudioEngine::close();

            break;
        }
        SDL_GL_SwapWindow(window);
    }
    delete player;

    return 0;
}

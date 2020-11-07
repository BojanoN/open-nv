
#include <util/camera.hpp>
#include <util/shader.hpp>

#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f
};

int main(void)
{

    unsigned int VBO;
    unsigned int VAO;

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

    SDL_Window*   window  = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_CaptureMouse(SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glewInit();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);

    Shader sh("camera.vs", "camera.fs");

    sh.activate();

    glm::vec3 pos { 0.0, 0.0, 0.0 };
    glm::vec3 up { 0.0, 1.0, 0.0 };
    glm::vec3 f { 0.0, 0.0, -1.0 };

    Util::Camera camera { pos, up, f };

    glm::mat4 model { 10.0 };
    glm::mat4 projection = glm::perspective(glm::radians(45.0), 800. / 600., 0.1, 100.0);

    unsigned int start = SDL_GetPerformanceCounter();
    unsigned int end   = 0;

    glm::mat4 view;

    while (true) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_GL_DeleteContext(context);
                    SDL_DestroyWindow(window);
                    SDL_Quit();

                    return 0;
                } else {
                    camera.handleKeyboard(event.key.keysym.sym, end - start);
                }
                break;
            case SDL_MOUSEMOTION:
                camera.handleMouse(event.motion.xrel, event.motion.yrel);
                break;
            }
        }
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.getViewMatrix();

        sh.activate();
        sh.setMat4("projection", projection, false);
        sh.setMat4("view", view, false);
        start = end;

        glBindVertexArray(VAO);

        sh.setMat4("model", model, false);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        SDL_GL_SwapWindow(window);

        end = SDL_GetPerformanceCounter();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

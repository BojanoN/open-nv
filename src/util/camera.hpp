#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

namespace Util {

class Camera {
public:
    Camera(glm::vec3& pos, glm::vec3& u, glm::vec3& f, float pt, float y, float s)
        : position(pos)
        , up(u)
        , front(f)
        , right()
        , pitch(pt)
        , yaw(y)
        , sensitivity(s)
    {
        update();
    }

    Camera(glm::vec3& pos, glm::vec3& u, glm::vec3& f)
        : position(pos)
        , up(u)
        , front(f)
        , right()
        , pitch(0.)
        , yaw(-90.)
        , sensitivity(0.2)
    {
        update();
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + front, localUp);
    }

    void handleKeyboard(int sym, float delta)
    {

        float v = speed; //* delta;

        if (sym == SDLK_w) {
            position += front * v;
        } else if (sym == SDLK_a) {
            position -= right * v;
        } else if (sym == SDLK_s) {
            position -= front * v;
        } else if (sym == SDLK_d) {
            position += right * v;
        }

        update();
    }

    void handleMouse(float xOffset, float yOffset)
    {
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0) {
            pitch = 89.0;
        } else if (pitch < -89.0) {
            pitch = -89.0;
        }

        update();
    }

private:
    void update()
    {
        float yaw_rad   = glm::radians(yaw);
        float pitch_rad = glm::radians(pitch);

        front.x = cos(yaw_rad) * cos(pitch_rad);
        front.y = sin(pitch_rad);
        front.z = sin(yaw_rad) * cos(pitch_rad);

        front   = glm::normalize(front);
        right   = glm::normalize(glm::cross(front, up));
        localUp = glm::normalize(glm::cross(right, front));
    }

    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 localUp;
    glm::vec3 front;
    glm::vec3 right;

    glm::mat4 view;
    glm::mat4 projection;

    float pitch;
    float yaw;

    float sensitivity;
    float speed = 0.2;

    glm::mat4 viewMatrix;
};
}

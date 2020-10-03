#pragma once

#include <math.h>
#include <math/mat4.hpp>
#include <math/quat.hpp>
#include <math/util.hpp>
#include <math/vec3.hpp>

#include <SDL2/SDL.h>

namespace Util {

class Camera {
public:
    Camera(Vector3& pos, Vector3& u, Vector3& f, float pt, float y, float s)
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

    Camera(Vector3& pos, Vector3& u, Vector3& f)
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

    Matrix44 getViewMatrix()
    {
        Vector3 tmp = position + front;
        return lookat_mat(position, tmp, up);

        //   return viewMatrix;
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
        float yaw_rad   = deg2rad(yaw);
        float pitch_rad = deg2rad(pitch);

        front[0] = cos(yaw_rad) * cos(pitch_rad);
        front[1] = sin(pitch_rad);
        front[2] = sin(yaw_rad) * cos(pitch_rad);

        front.normalize();

        right = front.cross(up);
        right.normalize();
        up = right.cross(front);
        up.normalize();

        up.print();
        right.print();
        front.print();
    }

    Vector3 position;
    Vector3 up;
    Vector3 front;
    Vector3 right;

    Matrix44 view;
    Matrix44 projection;

    float pitch;
    float yaw;

    float sensitivity;
    float speed = 0.2;

    Quaternion orientation;

    Matrix44 viewMatrix;
};
}

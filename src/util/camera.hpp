#pragma once

#include <math.h>
#include <math/mat4.hpp>
#include <math/quat.hpp>
#include <math/util.hpp>
#include <math/vec3.hpp>

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

    Matrix44& getViewMatrix()
    {
        return viewMatrix;
    }

    void update()
    {
        float yaw_rad   = deg2rad(yaw);
        float pitch_rad = deg2rad(pitch);

        front[0] = cos(yaw_rad) * cos(pitch_rad);
        front[1] = sin(pitch_rad);
        front[2] = cos(yaw_rad) * cos(pitch_rad);

        front.normalize();

        right = front.cross(up);
        right.normalize();
        up = right.cross(front);
        up.normalize();

        Vector3 tmp      = position + front;
        this->viewMatrix = lookat_mat(position, tmp, up);
    }

    /*void handleKeyboard(const EventPtr& e, float delta)
    {
        if (e->type == EventType::KEY) {
            std::shared_ptr<KeyEvent> ek = std::dynamic_pointer_cast<KeyEvent>(e);

            float v = speed * delta;

            if (ek->key == Key::KEY_W && ek->action == KeyAction::PRESSED) {
                position += front * v;
            } else if (ek->key == Key::KEY_A && ek->action == KeyAction::PRESSED) {
                position -= right * v;
            } else if (ek->key == Key::KEY_S && ek->action == KeyAction::PRESSED) {
                position -= front * v;
            } else if (ek->key == Key::KEY_D && ek->action == KeyAction::PRESSED) {
                position += right * v;
            }
        }
        update();
    }*/
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

    Matrix44 viewMatrix;
};
}

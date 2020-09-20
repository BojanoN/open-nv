#pragma once

#include <math.h>
#include <math/mat4.hpp>
#include <math/quat.hpp>
#include <math/util.hpp>
#include <math/vec3.hpp>

#include <window/events.hpp>

namespace Util {

class Camera {
public:
    Camera(Vector3& pos, Vector3& u, Vector3& f, double pt, double y, double s)
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

    void update()
    {
        double yaw_rad   = deg2rad(yaw);
        double pitch_rad = deg2rad(pitch);

        front[0] = cos(yaw_rad) * cos(pitch_rad);
        front[1] = sin(pitch_rad);
        front[2] = cos(yaw_rad) * cos(pitch_rad);

        front.normalize();

        right = front.cross(up);
        right.normalize();
        up = right.cross(front);
    }

    // This is called from the callback for glfw key events
    void handleKeyboard(KeyEvent& e, double delta)
    {
        double v = speed * delta;

        if (e.key == Key::KEY_W && e.action == KeyAction::PRESSED) {
            position += front * v;
        } else if (e.key == Key::KEY_A && e.action == KeyAction::PRESSED) {
            position -= right * v;
        } else if (e.key == Key::KEY_S && e.action == KeyAction::PRESSED) {
            position -= front * v;
        } else if (e.key == Key::KEY_D && e.action == KeyAction::PRESSED) {
            position += right * v;
        }

        update();
    }

    void handleMouse(double xOffset, double yOffset)
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

    double pitch;
    double yaw;

    double sensitivity;
    double speed = 0.2;
};
}

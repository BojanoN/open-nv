#pragma once

#include "constants.hpp"
#include "mat2.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

#include <math.h>

inline int mat2_equal(Matrix22 a, Matrix22 b)
{
    return (abs(a[0][0] - b[0][0]) <= Math::epsilon) && (abs(a[0][1] - b[0][1]) <= Math::epsilon) && (abs(a[1][0] - b[1][0]) <= Math::epsilon) && (abs(a[1][1] - b[1][1]) <= Math::epsilon);
}

inline int mat3_equal(Matrix33 a, Matrix33 b)
{
    return (abs(a[0][0] - b[0][0]) <= Math::epsilon) && (abs(a[0][1] - b[0][1]) <= Math::epsilon) && (abs(a[0][2] - b[0][2]) <= Math::epsilon) && (abs(a[1][0] - b[1][0]) <= Math::epsilon) && (abs(a[1][1] - b[1][1]) <= Math::epsilon) && (abs(a[1][2] - b[1][2]) <= Math::epsilon) && (abs(a[2][0] - b[2][0]) <= Math::epsilon) && (abs(a[2][1] - b[2][1]) <= Math::epsilon) && (abs(a[2][2] - b[2][2]) <= Math::epsilon);
}
inline int mat4_equal(Matrix44 a, Matrix44 b)
{
    return (abs(a[0][0] - b[0][0]) <= Math::epsilon) && (abs(a[0][1] - b[0][1]) <= Math::epsilon) && (abs(a[0][2] - b[0][2]) <= Math::epsilon) && (abs(a[0][3] - b[0][3]) <= Math::epsilon) && (abs(a[1][0] - b[1][0]) <= Math::epsilon) && (abs(a[1][1] - b[1][1]) <= Math::epsilon) && (abs(a[1][2] - b[1][2]) <= Math::epsilon) && (abs(a[1][3] - b[1][3]) <= Math::epsilon) && (abs(a[2][0] - b[2][0]) <= Math::epsilon) && (abs(a[2][1] - b[2][1]) <= Math::epsilon) && (abs(a[2][2] - b[2][2]) <= Math::epsilon) && (abs(a[2][3] - b[2][3]) <= Math::epsilon) && (abs(a[3][0] - b[3][0]) <= Math::epsilon) && (abs(a[3][1] - b[3][1]) <= Math::epsilon) && (abs(a[3][2] - b[3][2]) <= Math::epsilon) && (abs(a[3][3] - b[3][3]) <= Math::epsilon);
}

inline Matrix44 perspective_mat(float fovy, float aspect, float near, float far)
{
    Matrix44 mat;

    float diff = near - far;
    float f    = 1. / tan(fovy * 0.5);

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[0][3] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[1][3] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[2][3] = 0.;
    mat[3][0] = 0.;
    mat[3][1] = 0.;

    mat[0][0] = f / aspect;
    mat[1][1] = f;
    mat[2][2] = (far + near) / diff;
    mat[3][2] = (2 * far * near) / diff;
    mat[2][3] = -1.;

    return mat;
}

inline Matrix44 lookat_mat(Vector3& eye, Vector3& center, Vector3& up)
{
    Matrix44 mat;

    Vector3 F = (center - eye);
    Vector3 u;
    Vector3 s;

    F.normalize();

    s = F.cross(up);

    s.normalize();

    u = s.cross(F);

    mat[0][0] = s[0];
    mat[0][1] = u[0];
    mat[0][2] = -F[0];
    mat[0][3] = 0.;

    mat[1][0] = s[1];
    mat[1][1] = u[1];
    mat[1][2] = -F[1];
    mat[1][3] = 0.;

    mat[2][0] = s[2];
    mat[2][1] = u[2];
    mat[2][2] = -F[2];
    mat[2][3] = 0.;

    mat[3][0] = -(s.dot(eye));
    mat[3][1] = -(u.dot(eye));
    mat[3][2] = (F.dot(eye));
    mat[3][3] = 1.;

    return mat;
}

inline Matrix44 ortho_mat(float left, float right, float bottom, float top, float near, float far)
{
    Matrix44 mat;

    float rl_diff = right - left;
    float tb_diff = top - bottom;
    float fn_diff = far - near;

    float tx = -(right + left) / rl_diff;
    float ty = -(top + bottom) / tb_diff;
    float tz = -(far + near) / fn_diff;

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[0][3] = 0.;
    mat[1][3] = 0.;
    mat[2][3] = 0.;

    mat[0][0] = 2. / rl_diff;
    mat[1][1] = 2. / tb_diff;
    mat[2][2] = -2. / fn_diff;
    mat[3][0] = tx;
    mat[3][1] = ty;
    mat[3][2] = tz;
    mat[3][3] = 1.;

    return mat;
}

// Near and far are -1 and 1
inline Matrix44 ortho2D_mat(float left, float right, float bottom, float top)
{
    Matrix44 mat;

    float rl_diff = right - left;
    float tb_diff = top - bottom;

    float tx = -(right + left) / rl_diff;
    float ty = -(top + bottom) / tb_diff;

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[2][3] = 0.;
    mat[0][3] = 0.;
    mat[1][3] = 0.;
    mat[3][2] = 0.;

    mat[0][0] = 2. / rl_diff;
    mat[1][1] = 2. / tb_diff;
    mat[2][2] = -1.;
    mat[3][3] = 1.;

    mat[3][0] = tx;
    mat[3][1] = ty;

    return mat;
}

inline Matrix44 translate_mat(Vector3& vec)
{
    Matrix44 mat;

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[0][3] = 0.;
    mat[1][3] = 0.;
    mat[2][3] = 0.;

    mat[0][0] = 1.;
    mat[1][1] = 1.;
    mat[2][2] = 1.;
    mat[3][3] = 1.;

    mat[0][3] = vec[0];
    mat[1][3] = vec[1];
    mat[2][3] = vec[2];

    return mat;
}

inline Matrix44 translate_mat(float x, float y, float z)
{
    Matrix44 mat;

    mat[0][0] = 1.;
    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[0][3] = x;

    mat[1][0] = 0.;
    mat[1][1] = 1.;
    mat[1][2] = 0.;
    mat[1][3] = y;

    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[2][2] = 1.;
    mat[2][3] = z;

    mat[3][3] = 1.;

    return mat;
}

inline Matrix44 scale_mat(float val)
{
    Matrix44 mat;

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[0][3] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[1][3] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[3][0] = 0.;
    mat[3][1] = 0.;
    mat[3][2] = 0.;

    mat[0][0] = val;
    mat[1][1] = val;
    mat[2][2] = val;
    mat[3][3] = 1.;

    return mat;
}

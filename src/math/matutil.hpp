#pragma once

#include "constants.hpp"
#include "mat2.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

#include <math.h>

inline int mat2_equal(mat2 a, mat2 b)
{
    return (abs(a[0][0] - b[0][0]) <= Math::epsilon) && (abs(a[0][1] - b[0][1]) <= Math::epsilon) && (abs(a[1][0] - b[1][0]) <= Math::epsilon) && (abs(a[1][1] - b[1][1]) <= Math::epsilon);
}

inline int mat3_equal(mat3 a, mat3 b)
{
    return (abs(a[0][0] - b[0][0]) <= Math::epsilon) && (abs(a[0][1] - b[0][1]) <= Math::epsilon) && (abs(a[0][2] - b[0][2]) <= Math::epsilon) && (abs(a[1][0] - b[1][0]) <= Math::epsilon) && (abs(a[1][1] - b[1][1]) <= Math::epsilon) && (abs(a[1][2] - b[1][2]) <= Math::epsilon) && (abs(a[2][0] - b[2][0]) <= Math::epsilon) && (abs(a[2][1] - b[2][1]) <= Math::epsilon) && (abs(a[2][2] - b[2][2]) <= Math::epsilon);
}
inline int mat4_equal(mat4 a, mat4 b)
{
    return (abs(a[0][0] - b[0][0]) <= Math::epsilon) && (abs(a[0][1] - b[0][1]) <= Math::epsilon) && (abs(a[0][2] - b[0][2]) <= Math::epsilon) && (abs(a[0][3] - b[0][3]) <= Math::epsilon) && (abs(a[1][0] - b[1][0]) <= Math::epsilon) && (abs(a[1][1] - b[1][1]) <= Math::epsilon) && (abs(a[1][2] - b[1][2]) <= Math::epsilon) && (abs(a[1][3] - b[1][3]) <= Math::epsilon) && (abs(a[2][0] - b[2][0]) <= Math::epsilon) && (abs(a[2][1] - b[2][1]) <= Math::epsilon) && (abs(a[2][2] - b[2][2]) <= Math::epsilon) && (abs(a[2][3] - b[2][3]) <= Math::epsilon) && (abs(a[3][0] - b[3][0]) <= Math::epsilon) && (abs(a[3][1] - b[3][1]) <= Math::epsilon) && (abs(a[3][2] - b[3][2]) <= Math::epsilon) && (abs(a[3][3] - b[3][3]) <= Math::epsilon);
}

inline mat4 perspective_mat(double fovy, double aspect, double near, double far)
{
    mat4 mat;

    double diff = near - far;
    double f    = 1. / tan(fovy * 0.5);

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
    mat[2][3] = (2 * far * near) / diff;
    mat[3][2] = -1.;

    return mat;
}

inline mat4 lookat_mat(vec3& eye, vec3& center, vec3& up)
{
    mat4 mat;

    vec3 F = (center - eye);
    vec3 U = up;
    vec3 u;
    vec3 s;

    U.normalize();
    F.normalize();

    s      = F.cross(U);
    mat[0] = s;

    s.normalize();
    u = s.cross(F);
    F.negate();

    mat[1] = F;
    mat[2] = u;

    mat[0][3] = 0.;
    mat[1][3] = 0.;
    mat[2][3] = 0.;
    mat[3][0] = 0.;
    mat[3][1] = 0.;

    mat[3][3] = 1.;

    return mat;
}

inline mat4 ortho_mat(double left, double right, double bottom, double top, double near, double far)
{
    mat4 mat;

    double rl_diff = right - left;
    double tb_diff = top - bottom;
    double fn_diff = far - near;

    double tx = -(right + left) / rl_diff;
    double ty = -(top + bottom) / tb_diff;
    double tz = -(far + near) / fn_diff;

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[3][0] = 0.;
    mat[3][1] = 0.;
    mat[3][2] = 0.;

    mat[0][0] = 2. / rl_diff;
    mat[1][1] = 2. / tb_diff;
    mat[2][2] = -2. / fn_diff;
    mat[3][3] = 1.;

    mat[0][3] = tx;
    mat[1][3] = ty;
    mat[2][3] = tz;

    return mat;
}

// Near and far are -1 and 1
inline mat4 ortho2D_mat(double left, double right, double bottom, double top)
{
    mat4 mat;

    double rl_diff = right - left;
    double tb_diff = top - bottom;

    double tx = -(right + left) / rl_diff;
    double ty = -(top + bottom) / tb_diff;

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[2][3] = 0.;
    mat[3][0] = 0.;
    mat[3][1] = 0.;
    mat[3][2] = 0.;

    mat[0][0] = 2. / rl_diff;
    mat[1][1] = 2. / tb_diff;
    mat[2][2] = -1.;
    mat[3][3] = 1.;

    mat[0][3] = tx;
    mat[1][3] = ty;

    return mat;
}

inline mat4 translate_mat(vec3& vec)
{
    mat4 mat;

    mat[0][1] = 0.;
    mat[0][2] = 0.;
    mat[1][0] = 0.;
    mat[1][2] = 0.;
    mat[2][0] = 0.;
    mat[2][1] = 0.;
    mat[3][0] = 0.;
    mat[3][1] = 0.;
    mat[3][2] = 0.;

    mat[0][0] = 1.;
    mat[1][1] = 1.;
    mat[2][2] = 1.;
    mat[3][3] = 1.;

    mat[0][3] = vec[0];
    mat[1][3] = vec[1];
    mat[2][3] = vec[2];

    return mat;
}

inline mat4 scale_mat(double val)
{
    mat4 mat;

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

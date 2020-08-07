#pragma once

#include "constants.hpp"
#include "types.hpp"
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

inline void perspective_mat(mat4 mat, double fovy, double aspect, double near, double far)
{
    double diff = near - far;
    double f    = 1. / tan(fovy * 0.5);

    mat[0][0] = f / aspect;
    mat[1][1] = f;
    mat[2][2] = (far + near) / diff;
    mat[2][3] = (2 * far * near) / diff;
    mat[3][2] = -1.;
}

inline void lookat_mat(mat4 mat, vec3 eye, vec3 center, vec3 up)
{
    vec3 F;
    vec3 U;
    vec3 u;
    vec3 s;

    vec3_copy(up, U);
    vec3_sub(center, eye, F);

    vec3_normalize(F);
    vec3_normalize(U);

    vec3_cross(F, U, s);

    vec3_copy(s, mat[0]);

    vec3_normalize(s);
    vec3_cross(s, F, u);
    vec3_neg(F);

    vec3_copy(F, mat[1]);
    vec3_copy(u, mat[2]);

    mat[3][3] = 1.;
}

inline void ortho_mat(mat4 mat, double left, double right, double bottom, double top, double near, double far)
{
    double rl_diff = right - left;
    double tb_diff = top - bottom;
    double fn_diff = far - near;

    double tx = -(right + left) / rl_diff;
    double ty = -(top + bottom) / tb_diff;
    double tz = -(far + near) / fn_diff;

    mat[0][0] = 2. / rl_diff;
    mat[1][1] = 2. / tb_diff;
    mat[2][2] = -2. / fn_diff;
    mat[3][3] = 1.;

    mat[0][3] = tx;
    mat[1][3] = ty;
    mat[2][3] = tz;
}

// Near and far are -1 and 1
inline void ortho2D_mat(mat4 mat, double left, double right, double bottom, double top)
{
    double rl_diff = right - left;
    double tb_diff = top - bottom;

    double tx = -(right + left) / rl_diff;
    double ty = -(top + bottom) / tb_diff;

    mat[0][0] = 2. / rl_diff;
    mat[1][1] = 2. / tb_diff;
    mat[2][2] = -1.;
    mat[3][3] = 1.;

    mat[0][3] = tx;
    mat[1][3] = ty;
}

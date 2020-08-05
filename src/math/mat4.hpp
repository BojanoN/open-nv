#pragma once
#include "vec4.hpp"

inline void mat4_copy(mat4 src, mat4 dst)
{
    vec3_copy(src[0], dst[0]);
    vec3_copy(src[1], dst[1]);
    vec3_copy(src[2], dst[2]);
    vec3_copy(src[3], dst[3]);
}

#define vec4_copy(src, dst) vec4_init((src), (dst))

inline void mat4_diag(mat4 mat, float val)
{
    mat[0][0] = val;
    mat[1][1] = val;
    mat[2][2] = val;
    mat[3][3] = val;
}

#define mat4_ident(mat) mat4_diag((mat), 1.)

inline void mat4_scale(mat4 mat, float val)
{
    mat[0][0] *= val;
    mat[0][1] *= val;
    mat[0][2] *= val;
    mat[0][3] *= val;
    mat[1][0] *= val;
    mat[1][1] *= val;
    mat[1][2] *= val;
    mat[1][3] *= val;
    mat[2][0] *= val;
    mat[2][1] *= val;
    mat[2][2] *= val;
    mat[2][3] *= val;
    mat[3][0] *= val;
    mat[3][1] *= val;
    mat[3][2] *= val;
    mat[3][3] *= val;
}

inline void mat4_mul(mat4 a, mat4 b, mat4 dst)
{
    dst[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0];
    dst[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1];
    dst[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2];
    dst[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3];

    dst[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0];
    dst[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1];
    dst[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2];
    dst[1][3] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][3] + a[1][3] * b[3][3];

    dst[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0];
    dst[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1];
    dst[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2];
    dst[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3];

    dst[3][0] = a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0];
    dst[3][1] = a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1];
    dst[3][2] = a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2];
    dst[3][3] = a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3];
}

inline void mat4_colswap(mat4 mat, unsigned int c1, unsigned int c2)
{
    vec4 tmpcol;

    tmpcol[0] = mat[0][c1];
    tmpcol[1] = mat[1][c1];
    tmpcol[2] = mat[2][c1];
    tmpcol[3] = mat[3][c1];

    mat[0][c1] = mat[0][c2];
    mat[1][c1] = mat[1][c2];
    mat[2][c1] = mat[2][c2];
    mat[3][c1] = mat[3][c2];

    mat[0][c2] = tmpcol[0];
    mat[1][c2] = tmpcol[1];
    mat[2][c2] = tmpcol[2];
    mat[3][c2] = tmpcol[3];
}

inline void mat4_rowswap(mat4 mat, unsigned int r1, unsigned int r2)
{
    vec4 tmprow;

    vec4_copy(mat[r1], tmprow);
    vec4_copy(mat[r2], mat[r1]);
    vec4_copy(tmprow, mat[r2]);
}

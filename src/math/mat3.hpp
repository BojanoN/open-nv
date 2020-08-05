#pragma once
#include "types.hpp"
#include "vec3.hpp"

inline void mat3_copy(mat3 src, mat3 dst)
{
    vec3_copy(src[0], dst[0]);
    vec3_copy(src[1], dst[1]);
    vec3_copy(src[2], dst[2]);
}

inline void mat3_diag(mat3 mat, float val)
{
    mat[0][0] = val;
    mat[1][1] = val;
    mat[2][2] = val;
}

inline void mat3_scale(mat3 mat, float val)
{
    mat[0][0] *= val;
    mat[0][1] *= val;
    mat[0][2] *= val;
    mat[1][0] *= val;
    mat[1][1] *= val;
    mat[1][2] *= val;
    mat[2][0] *= val;
    mat[2][1] *= val;
    mat[2][2] *= val;
}

#define mat3_ident(mat) mat3_diag((mat), 1.)

inline void mat3_mul(mat3 a, mat3 b, mat3 dst)
{
    dst[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0];
    dst[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1];
    dst[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2];

    dst[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0];
    dst[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1];
    dst[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2];

    dst[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0];
    dst[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1];
    dst[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2];
}

inline void mat3_vec_mul(vec3 a, mat3 b, vec2 dst)
{
    dst[0] = a[0] * b[0][0] + a[1] * b[1][0] + a[2] * b[2][0];
    dst[1] = a[0] * b[0][1] + a[1] * b[1][1] + a[2] * b[2][1];
    dst[2] = a[0] * b[0][2] + a[1] * b[1][2] + a[2] * b[2][2];
}

inline float mat3_det(mat3 mat)
{
    float m00 = mat[0][0];
    float m01 = mat[0][1];
    float m02 = mat[0][2];
    float m10 = mat[1][0];
    float m11 = mat[1][1];
    float m12 = mat[1][2];
    float m20 = mat[2][0];
    float m21 = mat[2][1];
    float m22 = mat[2][2];

    return m00 * (m11 * m22 - m21 * m12) - m10 * (m01 * m22 - m02 * m21) + m20 * (m02 * m12 - m02 * m11);
}

inline void mat3_inv(mat3 mat)
{
    float m00 = mat[0][0];
    float m01 = mat[0][1];
    float m02 = mat[0][2];
    float m10 = mat[1][0];
    float m11 = mat[1][1];
    float m12 = mat[1][2];
    float m20 = mat[2][0];
    float m21 = mat[2][1];
    float m22 = mat[2][2];

    mat[0][0] = m11 * m22 - m12 * m21;
    mat[0][1] = -(m01 * m22 - m21 * m02);
    mat[0][2] = m01 * m12 - m11 * m02;
    mat[1][0] = -(m10 * m22 - m20 * m12);
    mat[1][1] = m00 * m22 - m02 * m20;
    mat[1][2] = -(m00 * m12 - m10 * m02);
    mat[2][0] = m10 * m21 - m20 * m11;
    mat[2][1] = -(m00 * m21 - m20 * m01);
    mat[2][2] = m00 * m11 - m01 * m10;

    float div = 1.f / (m00 * mat[0][0] + m01 * mat[1][0] + m02 * mat[2][0]);

    mat3_scale(mat, div);
}

inline void mat3_trans(mat3 mat)
{
    mat3 tmp;

    tmp[0][1] = mat[1][0];
    tmp[0][2] = mat[2][0];
    tmp[1][0] = mat[0][1];
    tmp[1][2] = mat[2][1];
    tmp[2][0] = mat[0][2];
    tmp[2][1] = mat[1][2];

    mat[0][1] = tmp[0][1];
    mat[0][2] = tmp[0][2];
    mat[1][0] = tmp[1][0];
    mat[1][2] = tmp[1][2];
    mat[2][0] = tmp[2][0];
    mat[2][1] = tmp[2][1];
}

inline void mat3_colswap(mat3 mat, unsigned int c1, unsigned int c2)
{
    vec3 tmpcol;

    tmpcol[0] = mat[0][c1];
    tmpcol[1] = mat[1][c1];
    tmpcol[2] = mat[2][c1];

    mat[0][c1] = mat[0][c2];
    mat[1][c1] = mat[1][c2];
    mat[2][c1] = mat[2][c2];

    mat[0][c2] = tmpcol[0];
    mat[1][c2] = tmpcol[1];
    mat[2][c2] = tmpcol[2];
}

inline void mat3_rowswap(mat3 mat, unsigned int r1, unsigned int r2)
{
    vec3 tmprow;

    vec3_copy(mat[r1], tmprow);
    vec3_copy(mat[r2], mat[r1]);
    vec3_copy(tmprow, mat[r2]);
}

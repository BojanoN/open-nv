#pragma once
#include "vec4.hpp"

inline void mat4_copy(mat4 src, mat4 dst)
{
    vec4_copy(src[0], dst[0]);
    vec4_copy(src[1], dst[1]);
    vec4_copy(src[2], dst[2]);
    vec4_copy(src[3], dst[3]);
}

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

inline float mat4_det(mat4 mat)
{
    float m00 = mat[0][0];
    float m01 = mat[0][1];
    float m02 = mat[0][2];
    float m03 = mat[0][3];
    float m10 = mat[1][0];
    float m11 = mat[1][1];
    float m12 = mat[1][2];
    float m13 = mat[1][3];
    float m20 = mat[2][0];
    float m21 = mat[2][1];
    float m22 = mat[2][2];
    float m23 = mat[2][3];
    float m30 = mat[3][0];
    float m31 = mat[3][1];
    float m32 = mat[3][2];
    float m33 = mat[3][3];

    float min1 = m22 * m33 - m32 * m23;
    float min2 = m21 * m33 - m31 * m23;
    float min3 = m21 * m32 - m31 * m22;
    float min4 = m20 * m33 - m30 * m23;
    float min5 = m20 * m32 - m30 * m22;
    float min6 = m20 * m31 - m30 * m21;

    return m00 * (m11 * min1 - m12 * min2 + m13 * min3)
        - m01 * (m10 * min1 - m12 * min4 + m13 * min5)
        + m02 * (m10 * min2 - m11 * min4 + m13 * min6)
        - m03 * (m10 * min3 - m11 * min5 + m12 * min6);
}

inline void mat4_inv(mat4 mat)
{
    float m00 = mat[0][0];
    float m01 = mat[0][1];
    float m02 = mat[0][2];
    float m03 = mat[0][3];
    float m10 = mat[1][0];
    float m11 = mat[1][1];
    float m12 = mat[1][2];
    float m13 = mat[1][3];
    float m20 = mat[2][0];
    float m21 = mat[2][1];
    float m22 = mat[2][2];
    float m23 = mat[2][3];
    float m30 = mat[3][0];
    float m31 = mat[3][1];
    float m32 = mat[3][2];
    float m33 = mat[3][3];

    float min1 = m22 * m33 - m32 * m23;
    float min2 = m21 * m33 - m31 * m23;
    float min3 = m21 * m32 - m31 * m22;
    float min4 = m20 * m33 - m30 * m23;
    float min5 = m20 * m32 - m30 * m22;
    float min6 = m20 * m31 - m30 * m21;

    mat[0][0] = m11 * min1 - m12 * min2 + m13 * min3;
    mat[1][0] = -(m10 * min1 - m12 * min4 + m13 * min5);
    mat[2][0] = m10 * min2 - m11 * min4 + m13 * min6;
    mat[3][0] = -(m10 * min3 - m11 * min5 + m12 * min6);

    mat[0][1] = -(m01 * min1 - m02 * min2 + m03 * min3);
    mat[1][1] = m00 * min1 - m02 * min4 + m03 * min5;
    mat[2][1] = -(m00 * min2 - m01 * min4 + m03 * min6);
    mat[3][1] = m00 * min3 - m01 * min5 + m02 * min6;

    min1 = m12 * m33 - m32 * m13;
    min2 = m11 * m33 - m31 * m13;
    min3 = m11 * m32 - m31 * m12;
    min4 = m10 * m33 - m30 * m13;
    min5 = m10 * m32 - m30 * m12;
    min6 = m10 * m31 - m30 * m11;

    mat[0][2] = m01 * min1 - m02 * min2 + m03 * min3;
    mat[1][2] = -(m00 * min1 - m02 * min4 + m03 * min5);
    mat[2][2] = m00 * min2 - m01 * min4 + m03 * min6;
    mat[3][2] = -(m00 * min3 - m01 * min5 + m02 * min6);

    min1 = m12 * m23 - m22 * m13;
    min2 = m11 * m23 - m21 * m13;
    min3 = m11 * m22 - m21 * m12;
    min4 = m10 * m23 - m20 * m13;
    min5 = m10 * m22 - m20 * m12;
    min6 = m10 * m21 - m20 * m11;

    mat[0][3] = -(m01 * min1 - m02 * min2 + m03 * min3);
    mat[1][3] = m00 * min1 - m02 * min4 + m03 * min5;
    mat[2][3] = -(m00 * min2 - m01 * min4 + m03 * min6);
    mat[3][3] = m00 * min3 - m01 * min5 + m02 * min6;

    float det = 1.0f / (m00 * mat[0][0] + m01 * mat[1][0] + m02 * mat[2][0] + m03 * mat[3][0]);

    mat4_scale(mat, det);
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
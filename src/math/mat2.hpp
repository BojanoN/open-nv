#pragma once
#include "types.hpp"

inline void mat2_copy(mat2 src, mat2 dst)
{
    dst[0][0] = src[0][0];
    dst[0][1] = src[0][1];
    dst[1][0] = src[1][0];
    dst[1][1] = src[1][1];
}

inline void mat2_diag(mat2 mat, double val)
{
    mat[0][1] = 0.;
    mat[1][0] = 0.;

    mat[0][0] = val;
    mat[1][1] = val;
}

#define mat2_ident(mat) mat2_diag((mat), 1.)

inline void mat2_mul(mat2 a, mat2 b, mat2 dst)
{
    dst[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
    dst[0][1] = a[0][0] * b[1][0] + a[0][1] * b[1][1];
    dst[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0];
    dst[0][1] = a[1][0] * b[1][0] + a[1][1] * b[1][1];
}

inline void mat2_vec_mul(vec2 a, mat2 b, vec2 dst)
{
    dst[0] = a[0] * b[0][0] + a[1] * b[1][0];
    dst[1] = a[0] * b[0][1] + a[1] * b[1][1];
}

inline double mat2_det(mat2 mat)
{
    return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
}

inline void mat2_inv(mat2 mat)
{
    double det = mat2_det(mat);
    double tmp = mat[0][0];

    mat[1][0] = (-mat[1][0]) / det;
    mat[0][1] = (-mat[0][1]) / det;
    mat[0][0] = mat[1][1] / det;
    mat[1][1] = tmp / det;
}

inline void mat2_trans(mat2 mat)
{
    double tmp = mat[1][0];

    mat[1][0] = mat[0][1];
    mat[0][1] = tmp;
}

inline void mat2_scale(mat2 src, double val)
{
    src[0][0] *= val;
    src[0][1] *= val;
    src[1][0] *= val;
    src[1][1] *= val;
}

inline void mat2_colswap(mat2 mat, unsigned int c1, unsigned int c2)
{
    vec2 tmpcol;

    tmpcol[0] = mat[0][c1];
    tmpcol[1] = mat[1][c1];

    mat[0][c1] = mat[0][c2];
    mat[1][c1] = mat[1][c2];
    mat[0][c2] = tmpcol[0];
    mat[1][c2] = tmpcol[1];
}

inline void mat2_rowswap(mat2 mat, unsigned int r1, unsigned int r2)
{
    vec2 tmprow;

    tmprow[0] = mat[r1][0];
    tmprow[1] = mat[r1][1];

    mat[r1][0] = mat[r2][0];
    mat[r1][1] = mat[r2][1];
    mat[r2][0] = tmprow[0];
    mat[r2][1] = tmprow[1];
}

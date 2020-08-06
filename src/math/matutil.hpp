#pragma once

#include "types.hpp"

inline int mat2_equal(mat2 a, mat2 b)
{
    return a[0][0] == b[0][0] && a[0][1] == b[0][1] && a[1][0] == b[1][0] && a[1][1] == b[1][1];
}

inline int mat3_equal(mat3 a, mat3 b)
{
    return a[0][0] == b[0][0] && a[0][1] == b[0][1] && a[1][0] == b[1][0] && a[1][1] == b[1][1] && a[0][0] == b[0][0] && a[0][1] == b[0][1] && a[0][2] == b[0][2] && a[1][0] == b[1][0] && a[1][1] == b[1][1] && a[1][2] == b[1][2] && a[2][0] == b[2][0] && a[2][1] == b[2][1] && a[2][2] == b[2][2];
}

inline int mat4_equal(mat4 a, mat4 b)
{
    return a[0][0] == b[0][0] && a[0][1] == b[0][1] && a[0][2] == b[0][2] && a[0][3] == b[0][3] && a[1][0] == b[1][0] && a[1][1] == b[1][1] && a[1][2] == b[1][2] && a[1][3] == b[1][3] && a[2][0] == b[2][0] && a[2][1] == b[2][1] && a[2][2] == b[2][2] && a[2][3] == b[2][3] && a[3][0] == b[3][0] && a[3][1] == b[3][1] && a[3][2] == b[3][2] && a[3][3] == b[3][3];
}

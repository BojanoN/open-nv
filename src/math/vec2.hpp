#pragma once
#include "types.hpp"
#include <math.h>
#include <stdio.h>

inline void vec2_init(double* in, vec2 vec)
{
    vec[0] = in[0];
    vec[1] = in[1];
}
inline void vec2_print(vec2 vec)
{
    printf("( %f %f )\n", vec[0], vec[1]);
}
inline void vec2_add(vec2 a, vec2 b, vec2 dst)
{
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
}
inline void vec2_addc(vec2 a, vec2 dst, double val)
{
    dst[0] = a[0] + val;
    dst[1] = a[1] + val;
}
inline void vec2_sub(vec2 a, vec2 b, vec2 dst)
{
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
}
inline void vec2_subc(vec2 a, vec2 dst, double val)
{
    dst[0] = a[0] - val;
    dst[1] = a[1] - val;
}
inline void vec2_mul(vec2 a, vec2 b, vec2 dst)
{
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
}
inline void vec2_mulc(vec2 a, vec2 dst, double val)
{
    dst[0] = a[0] * val;
    dst[1] = a[1] * val;
}
inline void vec2_div(vec2 a, vec2 b, vec2 dst)
{
    dst[0] = a[0] / b[0];
    dst[1] = a[1] / b[1];
}
inline void vec2_divc(vec2 a, vec2 dst, double val)
{
    dst[0] = a[0] / val;
    dst[1] = a[1] / val;
}

inline double vec2_dot(vec2 a, vec2 b)
{
    return a[0] * b[0] + a[1] * b[1];
}

inline double vec2_norm(vec2 a)
{
    return sqrt(a[0] * a[0] + a[1] * a[1]);
}

inline void vec2_normalize(vec2 a)
{
    double norm = vec2_norm(a);
    a[0] /= norm;
    a[1] /= norm;
}

inline void vec2_neg(vec2 a)
{
    a[0] = -a[0];
    a[1] = -a[1];
}

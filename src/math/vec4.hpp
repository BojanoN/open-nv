#pragma once
#include "types.hpp"
#include <math.h>
#include <stdio.h>

inline void vec4_init(double* in, vec4 vec)
{
    vec[0] = in[0];
    vec[1] = in[1];
    vec[2] = in[2];
    vec[3] = in[3];
}

#define vec4_copy(src, dst) vec4_init((src), (dst))

inline void vec4_print(vec4 vec)
{
    printf("( %f %f %f %f )\n", vec[0], vec[1], vec[2], vec[3]);
}
inline void vec4_add(vec4 a, vec4 b, vec4 dst)
{
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
    dst[3] = a[3] + b[3];
}
inline void vec4_addc(vec4 a, vec4 dst, double val)
{
    dst[0] = a[0] + val;
    dst[1] = a[1] + val;
    dst[2] = a[2] + val;
    dst[3] = a[3] + val;
}
inline void vec4_sub(vec4 a, vec4 b, vec4 dst)
{
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
    dst[3] = a[3] - b[3];
}
inline void vec4_subc(vec4 a, vec4 dst, double val)
{
    dst[0] = a[0] - val;
    dst[1] = a[1] - val;
    dst[2] = a[2] - val;
    dst[3] = a[3] - val;
}
inline void vec4_mul(vec4 a, vec4 b, vec4 dst)
{
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
    dst[2] = a[2] * b[2];
    dst[3] = a[3] * b[3];
}
inline void vec4_mulc(vec4 a, vec4 dst, double val)
{
    dst[0] = a[0] * val;
    dst[1] = a[1] * val;
    dst[2] = a[2] * val;
    dst[3] = a[3] * val;
}
inline void vec4_div(vec4 a, vec4 b, vec4 dst)
{
    dst[0] = a[0] / b[0];
    dst[1] = a[1] / b[1];
    dst[2] = a[2] / b[2];
    dst[3] = a[3] / b[3];
}
inline void vec4_divc(vec4 a, vec4 dst, double val)
{
    dst[0] = a[0] / val;
    dst[1] = a[1] / val;
    dst[2] = a[2] / val;
    dst[3] = a[3] / val;
}

inline double vec4_dot(vec4 a, vec4 b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

inline double vec4_norm(vec4 a)
{
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]);
}

inline void vec4_normalize(vec4 a)
{
    double norm = vec4_norm(a);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
    a[3] /= norm;
}

inline void vec4_neg(vec4 a)
{
    a[0] = -a[0];
    a[1] = -a[1];
    a[2] = -a[2];
    a[3] = -a[3];
}

#pragma once

#include "types.hpp"
#include <math.h>
#include <stdio.h>

inline void vec3_init(float* in, vec3 vec)
{
    vec[0] = in[0];
    vec[1] = in[1];
    vec[2] = in[2];
}

#define vec3_copy(src, dst) vec3_init((src), (dst))

inline void vec3_print(vec3 vec)
{
    printf("( %f %f %f )\n", vec[0], vec[1], vec[2]);
}
inline void vec3_add(vec3 a, vec3 b, vec3 dst)
{
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}
inline void vec3_addc(vec3 a, vec3 dst, float val)
{
    dst[0] = a[0] + val;
    dst[1] = a[1] + val;
    dst[2] = a[2] + val;
}
inline void vec3_sub(vec3 a, vec3 b, vec3 dst)
{
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
}
inline void vec3_subc(vec3 a, vec3 dst, float val)
{
    dst[0] = a[0] - val;
    dst[1] = a[1] - val;
    dst[2] = a[2] - val;
}
inline void vec3_mul(vec3 a, vec3 b, vec3 dst)
{
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
    dst[2] = a[2] * b[2];
}
inline void vec3_mulc(vec3 a, vec3 dst, float val)
{
    dst[0] = a[0] * val;
    dst[1] = a[1] * val;
    dst[2] = a[2] * val;
}
inline void vec3_div(vec3 a, vec3 b, vec3 dst)
{
    dst[0] = a[0] / b[0];
    dst[1] = a[1] / b[1];
    dst[2] = a[2] / b[2];
}
inline void vec3_divc(vec3 a, vec3 dst, float val)
{
    dst[0] = a[0] / val;
    dst[1] = a[1] / val;
    dst[2] = a[2] / val;
}

inline float vec3_dot(vec3 a, vec3 b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline float vec3_norm(vec3 a)
{
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

inline void vec3_normalize(vec3 a)
{
    float norm = vec3_norm(a);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

inline void vec3_cross(vec3 a, vec3 b, vec3 dst)
{
    dst[0] = a[1] * b[2] - b[1] * a[2];
    dst[1] = a[2] * b[0] - b[2] * a[0];
    dst[2] = a[0] * b[1] - b[0] * a[1];
}

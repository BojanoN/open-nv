#pragma once

#include "vec3.hpp"
#include <math.h>
#include <stdio.h>

struct vec4 {

    double& operator[](int index)
    {
        return arr[index];
    }

    vec4()
    {
        arr[0] = 0.;
        arr[1] = 0.;
        arr[2] = 0.;
        arr[3] = 0.;
    }

    vec4(double in[4])
    {
        arr[0] = in[0];
        arr[1] = in[1];
        arr[2] = in[2];
        arr[3] = in[3];
    }

    vec4(double x, double y, double z, double w)
    {
        arr[0] = x;
        arr[1] = y;
        arr[2] = z;
        arr[3] = w;
    }

    void vec4_print()
    {
        printf("( %f %f %f %f )\n", arr[0], arr[1], arr[2], arr[3]);
    }

    void operator=(vec3& vec)
    {
        arr[0] = vec[0];
        arr[1] = vec[1];
        arr[2] = vec[2];
    }

    vec4 operator+(vec4& b)
    {
        vec4 ret;

        ret[0] = arr[0] + b[0];
        ret[1] = arr[1] + b[1];
        ret[2] = arr[2] + b[2];
        ret[3] = arr[3] + b[3];

        return ret;
    }

    vec4 operator+(double val)
    {
        vec4 ret;

        ret[0] = arr[0] + val;
        ret[1] = arr[1] + val;
        ret[2] = arr[2] + val;
        ret[3] = arr[3] + val;

        return ret;
    }
    vec4 operator-(vec4& b)
    {
        vec4 ret;

        ret[0] = arr[0] - b[0];
        ret[1] = arr[1] - b[1];
        ret[2] = arr[2] - b[2];
        ret[3] = arr[3] - b[3];

        return ret;
    }

    vec4 operator-(double val)
    {
        vec4 ret;

        ret[0] = arr[0] - val;
        ret[1] = arr[1] - val;
        ret[2] = arr[2] - val;
        ret[3] = arr[3] - val;

        return ret;
    }
    vec4 operator*(vec4& b)
    {
        vec4 ret;

        ret[0] = arr[0] * b[0];
        ret[1] = arr[1] * b[1];
        ret[2] = arr[2] * b[2];
        ret[3] = arr[3] * b[3];

        return ret;
    }

    vec4 operator*(double val)
    {
        vec4 ret;

        ret[0] = arr[0] * val;
        ret[1] = arr[1] * val;
        ret[2] = arr[2] * val;
        ret[3] = arr[3] * val;

        return ret;
    }

    vec4 operator/(vec4& b)
    {
        vec4 ret;

        ret[0] = arr[0] / b[0];
        ret[1] = arr[1] / b[1];
        ret[2] = arr[2] / b[2];
        ret[3] = arr[3] / b[3];

        return ret;
    }
    vec4 operator/(double val)
    {
        vec4 ret;

        ret[0] = arr[0] / val;
        ret[1] = arr[1] / val;
        ret[2] = arr[2] / val;
        ret[3] = arr[3] / val;

        return ret;
    }

    void operator+=(vec4& b)
    {

        arr[0] += b[0];
        arr[1] += b[1];
        arr[2] += b[2];
        arr[3] += b[3];
    }

    void operator+=(double val)
    {

        arr[0] += val;
        arr[1] += val;
        arr[2] += val;
        arr[3] += val;
    }

    void operator-=(vec4& b)
    {
        arr[0] -= b[0];
        arr[1] -= b[1];
        arr[2] -= b[2];
        arr[3] -= b[3];
    }

    void operator-=(double val)
    {
        arr[0] -= val;
        arr[1] -= val;
        arr[2] -= val;
        arr[3] -= val;
    }

    void operator*=(vec4& b)
    {
        arr[0] *= b[0];
        arr[1] *= b[1];
        arr[2] *= b[2];
        arr[3] *= b[3];
    }

    void operator*=(double val)
    {
        arr[0] *= val;
        arr[1] *= val;
        arr[2] *= val;
        arr[3] *= val;
    }

    void operator/=(vec4& b)
    {
        arr[0] /= b[0];
        arr[1] /= b[1];
        arr[2] /= b[2];
        arr[3] /= b[3];
    }

    void operator/=(double val)
    {

        arr[0] /= val;
        arr[1] /= val;
        arr[2] /= val;
        arr[3] /= val;
    }

    double dot(vec4& b)
    {
        return arr[0] * b[0] + arr[1] * b[1] + arr[2] * b[2] + arr[3] * b[3];
    }

    double norm()
    {
        return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2] + arr[3] * arr[3]);
    }

    void normalize()
    {
        double len = norm();
        arr[0] /= len;
        arr[1] /= len;
        arr[2] /= len;
        arr[3] /= len;
    }

    void operator-()
    {
        arr[0] = -arr[0];
        arr[1] = -arr[1];
        arr[2] = -arr[2];
        arr[3] = -arr[3];
    }

    double arr[4];
};

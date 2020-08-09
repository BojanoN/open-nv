#pragma once

#include <math.h>
#include <stdio.h>

struct vec3 {

    double& operator[](int index)
    {
        return arr[index];
    }

    vec3()
    {
        arr[0] = 0.;
        arr[1] = 0.;
        arr[2] = 0.;
    }

    vec3(double in[3])
    {
        arr[0] = in[0];
        arr[1] = in[1];
        arr[2] = in[2];
    }

    vec3(double x, double y, double z)
    {
        arr[0] = x;
        arr[1] = y;
        arr[2] = z;
    }

    void vec3_print()
    {
        printf("( %f %f %f )\n", arr[0], arr[1], arr[2]);
    }

    vec3 operator+(vec3& b)
    {
        vec3 ret;

        ret[0] = arr[0] + b[0];
        ret[1] = arr[1] + b[1];
        ret[2] = arr[2] + b[2];

        return ret;
    }

    vec3 operator+(double val)
    {
        vec3 ret;

        ret[0] = arr[0] + val;
        ret[1] = arr[1] + val;
        ret[2] = arr[2] + val;

        return ret;
    }
    vec3 operator-(vec3& b)
    {
        vec3 ret;

        ret[0] = arr[0] - b[0];
        ret[1] = arr[1] - b[1];
        ret[2] = arr[2] - b[2];

        return ret;
    }
    vec3 operator-(double val)
    {
        vec3 ret;

        ret[0] = arr[0] - val;
        ret[1] = arr[1] - val;
        ret[2] = arr[2] - val;

        return ret;
    }
    vec3 operator*(vec3& b)
    {
        vec3 ret;

        ret[0] = arr[0] * b[0];
        ret[1] = arr[1] * b[1];
        ret[2] = arr[2] * b[2];

        return ret;
    }

    vec3 operator*(double val)
    {
        vec3 ret;

        ret[0] = arr[0] * val;
        ret[1] = arr[1] * val;
        ret[2] = arr[2] * val;

        return ret;
    }
    vec3 operator/(vec3& b)
    {
        vec3 ret;

        ret[0] = arr[0] / b[0];
        ret[1] = arr[1] / b[1];
        ret[2] = arr[2] / b[2];

        return ret;
    }

    vec3 operator/(double val)
    {
        vec3 ret;

        ret[0] = arr[0] / val;
        ret[1] = arr[1] / val;
        ret[2] = arr[2] / val;

        return ret;
    }

    void operator+=(vec3& b)
    {

        arr[0] += b[0];
        arr[1] += b[1];
        arr[2] += b[2];
    }

    void operator+=(double val)
    {

        arr[0] += val;
        arr[1] += val;
        arr[2] += val;
    }
    void operator-=(vec3& b)
    {

        arr[0] -= b[0];
        arr[1] -= b[1];
        arr[2] -= b[2];
    }
    void operator-=(double val)
    {

        arr[0] -= val;
        arr[1] -= val;
        arr[2] -= val;
    }

    void operator*=(vec3& b)
    {

        arr[0] *= b[0];
        arr[1] *= b[1];
        arr[2] *= b[2];
    }

    void operator*=(double val)
    {

        arr[0] *= val;
        arr[1] *= val;
        arr[2] *= val;
    }
    void operator/=(vec3& b)
    {

        arr[0] /= b[0];
        arr[1] /= b[1];
        arr[2] /= b[2];
    }

    void operator/=(double val)
    {

        arr[0] /= val;
        arr[1] /= val;
        arr[2] /= val;
    }

    double dot(vec3& b)
    {
        return arr[0] * b[0] + arr[1] * b[1] + arr[2] * b[2];
    }

    double norm()
    {
        return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
    }

    void normalize()
    {
        double len = norm();
        arr[0] /= len;
        arr[1] /= len;
        arr[2] /= len;
    }

    vec3 cross(vec3& b)
    {
        vec3 ret;

        ret[0] = arr[1] * b[2] - b[1] * arr[2];
        ret[1] = arr[2] * b[0] - b[2] * arr[0];
        ret[2] = arr[0] * b[1] - b[0] * arr[1];

        return ret;
    }

    void operator-()
    {
        arr[0] = -arr[0];
        arr[1] = -arr[1];
        arr[2] = -arr[2];
    }

    double arr[3];
};

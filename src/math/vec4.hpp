#pragma once

#include "vec3.hpp"
#include <math.h>
#include <stdio.h>

struct Vector4 {

    float& operator[](int index)
    {
        return arr[index];
    }

    const float& operator[](int index) const
    {
        return arr[index];
    }

    Vector4()
    {
        arr[0] = 0.;
        arr[1] = 0.;
        arr[2] = 0.;
        arr[3] = 0.;
    }

    Vector4(float in[4])
    {
        arr[0] = in[0];
        arr[1] = in[1];
        arr[2] = in[2];
        arr[3] = in[3];
    }

    Vector4(float x, float y, float z, float w)
    {
        arr[0] = x;
        arr[1] = y;
        arr[2] = z;
        arr[3] = w;
    }

    void Vector4_print()
    {
        printf("( %f %f %f %f )\n", arr[0], arr[1], arr[2], arr[3]);
    }

    void operator=(const Vector3& vec)
    {
        arr[0] = vec[0];
        arr[1] = vec[1];
        arr[2] = vec[2];
    }

    Vector4 operator+(const Vector4& b) const
    {
        Vector4 ret;

        ret[0] = arr[0] + b[0];
        ret[1] = arr[1] + b[1];
        ret[2] = arr[2] + b[2];
        ret[3] = arr[3] + b[3];

        return ret;
    }

    Vector4 operator+(float val) const
    {
        Vector4 ret;

        ret[0] = arr[0] + val;
        ret[1] = arr[1] + val;
        ret[2] = arr[2] + val;
        ret[3] = arr[3] + val;

        return ret;
    }
    Vector4 operator-(const Vector4& b) const
    {
        Vector4 ret;

        ret[0] = arr[0] - b[0];
        ret[1] = arr[1] - b[1];
        ret[2] = arr[2] - b[2];
        ret[3] = arr[3] - b[3];

        return ret;
    }

    Vector4 operator-(float val) const
    {
        Vector4 ret;

        ret[0] = arr[0] - val;
        ret[1] = arr[1] - val;
        ret[2] = arr[2] - val;
        ret[3] = arr[3] - val;

        return ret;
    }
    Vector4 operator*(const Vector4& b) const
    {
        Vector4 ret;

        ret[0] = arr[0] * b[0];
        ret[1] = arr[1] * b[1];
        ret[2] = arr[2] * b[2];
        ret[3] = arr[3] * b[3];

        return ret;
    }

    Vector4 operator*(float val) const
    {
        Vector4 ret;

        ret[0] = arr[0] * val;
        ret[1] = arr[1] * val;
        ret[2] = arr[2] * val;
        ret[3] = arr[3] * val;

        return ret;
    }

    Vector4 operator/(const Vector4& b) const
    {
        Vector4 ret;

        ret[0] = arr[0] / b[0];
        ret[1] = arr[1] / b[1];
        ret[2] = arr[2] / b[2];
        ret[3] = arr[3] / b[3];

        return ret;
    }
    Vector4 operator/(float val) const
    {
        Vector4 ret;

        ret[0] = arr[0] / val;
        ret[1] = arr[1] / val;
        ret[2] = arr[2] / val;
        ret[3] = arr[3] / val;

        return ret;
    }

    void operator+=(const Vector4& b)
    {

        arr[0] += b[0];
        arr[1] += b[1];
        arr[2] += b[2];
        arr[3] += b[3];
    }

    void operator+=(float val)
    {

        arr[0] += val;
        arr[1] += val;
        arr[2] += val;
        arr[3] += val;
    }

    void operator-=(const Vector4& b)
    {
        arr[0] -= b[0];
        arr[1] -= b[1];
        arr[2] -= b[2];
        arr[3] -= b[3];
    }

    void operator-=(float val)
    {
        arr[0] -= val;
        arr[1] -= val;
        arr[2] -= val;
        arr[3] -= val;
    }

    void operator*=(const Vector4& b)
    {
        arr[0] *= b[0];
        arr[1] *= b[1];
        arr[2] *= b[2];
        arr[3] *= b[3];
    }

    void operator*=(float val)
    {
        arr[0] *= val;
        arr[1] *= val;
        arr[2] *= val;
        arr[3] *= val;
    }

    void operator/=(const Vector4& b)
    {
        arr[0] /= b[0];
        arr[1] /= b[1];
        arr[2] /= b[2];
        arr[3] /= b[3];
    }

    void operator/=(float val)
    {

        arr[0] /= val;
        arr[1] /= val;
        arr[2] /= val;
        arr[3] /= val;
    }

    float dot(const Vector4& b) const
    {
        return arr[0] * b[0] + arr[1] * b[1] + arr[2] * b[2] + arr[3] * b[3];
    }

    float norm() const
    {
        return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2] + arr[3] * arr[3]);
    }

    void normalize()
    {
        float len = norm();
        arr[0] /= len;
        arr[1] /= len;
        arr[2] /= len;
        arr[3] /= len;
    }

    float cos(const Vector4& vec) const
    {
        return this->dot(vec) / (this->norm() * vec.norm());
    }

    Vector4 operator-()
    {
        Vector4 ret;

        ret[0] = -arr[0];
        ret[1] = -arr[1];
        ret[2] = -arr[2];
        ret[3] = -arr[3];

        return ret;
    }

    void negate()
    {
        arr[0] = -arr[0];
        arr[1] = -arr[1];
        arr[2] = -arr[2];
        arr[3] = -arr[3];
    }

    float arr[4];
};

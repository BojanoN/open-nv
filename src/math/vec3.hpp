#pragma once

#include <math.h>
#include <stdio.h>

struct Vector3 {

    float& operator[](int index)
    {
        return arr[index];
    }

    const float& operator[](int index) const
    {
        return arr[index];
    }

    Vector3()
    {
        arr[0] = 0.;
        arr[1] = 0.;
        arr[2] = 0.;
    }

    Vector3(float in[3])
    {
        arr[0] = in[0];
        arr[1] = in[1];
        arr[2] = in[2];
    }

    Vector3(float x, float y, float z)
    {
        arr[0] = x;
        arr[1] = y;
        arr[2] = z;
    }

    void Vector3_print()
    {
        printf("( %f %f %f )\n", arr[0], arr[1], arr[2]);
    }

    Vector3 operator+(const Vector3& b) const
    {
        Vector3 ret;

        ret[0] = arr[0] + b[0];
        ret[1] = arr[1] + b[1];
        ret[2] = arr[2] + b[2];

        return ret;
    }

    Vector3 operator+(float val) const
    {
        Vector3 ret;

        ret[0] = arr[0] + val;
        ret[1] = arr[1] + val;
        ret[2] = arr[2] + val;

        return ret;
    }
    Vector3 operator-(const Vector3& b) const
    {
        Vector3 ret;

        ret[0] = arr[0] - b[0];
        ret[1] = arr[1] - b[1];
        ret[2] = arr[2] - b[2];

        return ret;
    }
    Vector3 operator-(float val) const
    {
        Vector3 ret;

        ret[0] = arr[0] - val;
        ret[1] = arr[1] - val;
        ret[2] = arr[2] - val;

        return ret;
    }
    Vector3 operator*(const Vector3& b) const
    {
        Vector3 ret;

        ret[0] = arr[0] * b[0];
        ret[1] = arr[1] * b[1];
        ret[2] = arr[2] * b[2];

        return ret;
    }

    Vector3 operator*(float val) const
    {
        Vector3 ret;

        ret[0] = arr[0] * val;
        ret[1] = arr[1] * val;
        ret[2] = arr[2] * val;

        return ret;
    }
    Vector3 operator/(const Vector3& b) const
    {
        Vector3 ret;

        ret[0] = arr[0] / b[0];
        ret[1] = arr[1] / b[1];
        ret[2] = arr[2] / b[2];

        return ret;
    }

    Vector3 operator/(float val) const
    {
        Vector3 ret;

        ret[0] = arr[0] / val;
        ret[1] = arr[1] / val;
        ret[2] = arr[2] / val;

        return ret;
    }

    void operator+=(const Vector3& b)
    {

        arr[0] += b[0];
        arr[1] += b[1];
        arr[2] += b[2];
    }

    void operator+=(float val)
    {

        arr[0] += val;
        arr[1] += val;
        arr[2] += val;
    }
    void operator-=(const Vector3& b)
    {

        arr[0] -= b[0];
        arr[1] -= b[1];
        arr[2] -= b[2];
    }
    void operator-=(float val)
    {

        arr[0] -= val;
        arr[1] -= val;
        arr[2] -= val;
    }

    void operator*=(const Vector3& b)
    {

        arr[0] *= b[0];
        arr[1] *= b[1];
        arr[2] *= b[2];
    }

    void operator*=(float val)
    {

        arr[0] *= val;
        arr[1] *= val;
        arr[2] *= val;
    }
    void operator/=(const Vector3& b)
    {

        arr[0] /= b[0];
        arr[1] /= b[1];
        arr[2] /= b[2];
    }

    void operator/=(float val)
    {

        arr[0] /= val;
        arr[1] /= val;
        arr[2] /= val;
    }

    float dot(const Vector3& b) const
    {
        return arr[0] * b[0] + arr[1] * b[1] + arr[2] * b[2];
    }

    float norm() const
    {
        return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
    }

    float normSquared()
    {
        return arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2];
    }

    void normalize()
    {
        float len = norm();
        arr[0] /= len;
        arr[1] /= len;
        arr[2] /= len;
    }

    Vector3 cross(const Vector3& b) const
    {
        Vector3 ret;

        ret[0] = arr[1] * b[2] - b[1] * arr[2];
        ret[1] = arr[2] * b[0] - b[2] * arr[0];
        ret[2] = arr[0] * b[1] - b[0] * arr[1];

        return ret;
    }

    float cos(const Vector3& vec) const
    {
        return this->dot(vec) / (this->norm() * vec.norm());
    }

    Vector3 operator-()
    {
        Vector3 ret;

        ret[0] = -arr[0];
        ret[1] = -arr[1];
        ret[2] = -arr[2];

        return ret;
    }

    void negate()
    {
        arr[0] = -arr[0];
        arr[1] = -arr[1];
        arr[2] = -arr[2];
    }

    void print()
    {
        printf("(%f %f %f)\n", arr[0], arr[1], arr[2]);
    }

    float arr[3];
};

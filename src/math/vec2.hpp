#pragma once
#include <math.h>
#include <stdio.h>

struct Vector2 {

    Vector2()
    {
        arr[0] = 0.;
        arr[1] = 0.;
    }

    Vector2(double in[2])
    {
        arr[0] = in[0];
        arr[1] = in[1];
    }

    Vector2(double x, double y)
    {
        arr[0] = x;
        arr[1] = y;
    }

    double& operator[](int index)
    {
        return arr[index];
    }

    const double& operator[](int index) const
    {
        return arr[index];
    }

    void print()
    {
        printf("( %f %f )\n", arr[0], arr[1]);
    }

    Vector2 operator+(const Vector2& b) const
    {
        Vector2 ret;
        ret[0] = arr[0] + b[0];
        ret[1] = arr[1] + b[1];

        return ret;
    }

    Vector2 operator+(double val) const
    {
        Vector2 ret;

        ret[0] = arr[0] + val;
        ret[1] = arr[1] + val;

        return ret;
    }

    Vector2 operator-(const Vector2& b) const
    {
        Vector2 ret;
        ret[0] = arr[0] - b[0];
        ret[1] = arr[1] - b[1];
        return ret;
    }

    Vector2 operator-(double val) const
    {
        Vector2 ret;
        ret[0] = arr[0] - val;
        ret[1] = arr[1] - val;
        return ret;
    }

    void operator+=(Vector2& b)
    {
        arr[0] += b[0];
        arr[1] += b[1];
    }

    void operator+=(double val)
    {
        arr[0] += val;
        arr[1] += val;
    }

    void operator-=(Vector2& b)
    {
        arr[0] -= b[0];
        arr[1] -= b[1];
    }

    void operator-=(double val)
    {
        arr[0] -= val;
        arr[1] -= val;
    }

    Vector2 operator*(const Vector2& b) const
    {
        Vector2 ret;

        ret[0] = arr[0] * b[0];
        ret[1] = arr[1] * b[1];

        return ret;
    }

    Vector2 operator*(double val) const
    {
        Vector2 ret;

        ret[0] = arr[0] * val;
        ret[1] = arr[1] * val;

        return ret;
    }

    Vector2 operator/(const Vector2& b) const
    {
        Vector2 ret;

        ret[0] = arr[0] / b[0];
        ret[1] = arr[1] / b[1];

        return ret;
    }

    Vector2 operator/(double val) const
    {
        Vector2 ret;

        ret[0] = arr[0] / val;
        ret[1] = arr[1] / val;

        return ret;
    }

    void operator*=(Vector2& b)
    {
        arr[0] *= b[0];
        arr[1] *= b[1];
    }
    void operator*=(double val)
    {
        arr[0] *= val;
        arr[1] *= val;
    }
    void operator/=(Vector2& b)
    {
        arr[0] /= b[0];
        arr[1] /= b[1];
    }
    void operator/=(double val)
    {
        arr[0] /= val;
        arr[1] /= val;
    }

    double dot(const Vector2& b) const
    {
        return arr[0] * b[0] + arr[1] * b[1];
    }

    double norm() const
    {
        return sqrt(arr[0] * arr[0] + arr[1] * arr[1]);
    }

    void normalize()
    {
        double len = norm();
        arr[0] /= len;
        arr[1] /= len;
    }

    double cos(const Vector2& vec) const
    {
        return this->dot(vec) / (this->norm() * vec.norm());
    }

    Vector2 operator-()
    {
        Vector2 ret;

        ret[0] = -arr[0];
        ret[1] = -arr[1];

        return ret;
    }

    void negate()
    {
        arr[0] = -arr[0];
        arr[1] = -arr[1];
    }

    double arr[2];
};

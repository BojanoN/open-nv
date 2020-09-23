#pragma once

#include "mat4.hpp"
#include "vec3.hpp"

#include <math.h>

struct Quaternion {

    Quaternion()
        : scalar(0.)
    {
    }

    Quaternion(float angle, Vector3 vec)
        : scalar(cos(angle / 2))
        , vector(vec)
    {
        vector.normalize();
        vector *= sin(angle / 2);
    }

    void operator+=(const Quaternion& q)
    {
        scalar += q.scalar;
        vector += q.vector;
    }

    Quaternion operator+(const Quaternion& q) const
    {
        Quaternion ret;

        ret.scalar = scalar + q.scalar;
        ret.vector = vector + q.vector;

        return ret;
    }

    void operator-=(const Quaternion& q)
    {
        scalar -= q.scalar;
        vector -= q.vector;
    }

    Quaternion operator-(const Quaternion& q) const
    {
        Quaternion ret;

        ret.scalar = scalar - q.scalar;
        ret.vector = vector - q.vector;

        return ret;
    }

    Quaternion operator*(const Quaternion& q) const
    {
        Quaternion ret;

        ret.scalar = scalar * q.scalar - vector.dot(q.vector);
        ret.vector = (q.vector * scalar + vector * q.scalar) + vector.cross(q.vector);

        return ret;
    }

    void operator*=(const Quaternion& q)
    {
        vector = (q.vector * scalar + vector * q.scalar) + vector.cross(q.vector);
        scalar = scalar * q.scalar - vector.dot(q.vector);
    }

    Quaternion conjugate()
    {
        Quaternion ret;

        ret.scalar = scalar;
        ret.vector = -vector;

        return ret;
    }

    float normSquared()
    {
        return scalar * scalar + vector.normSquared();
    }

    float norm()
    {
        return sqrt(scalar * scalar + vector.normSquared());
    }

    Vector3 rotate(const Vector3& vec)
    {
        Vector3 ret;

        Vector3 t = (vector * 2).cross(vec);
        ret       = vec + (t * scalar) + vector.cross(t);

        return ret;
    }

    Matrix44 toMat4()
    {
        Matrix44 mat;

        float x = vector[0];
        float y = vector[1];
        float z = vector[2];
        float w = scalar;

        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yz = y * z;
        float yw = y * w;
        float zw = z * w;

        float x_sq = 2 * x * x;
        float y_sq = 2 * y * y;
        float z_sq = 2 * z * z;

        mat[0][0] = 1. - y_sq - z_sq;
        mat[1][1] = 1. - x_sq - z_sq;
        mat[2][2] = 1. - x_sq - y_sq;

        mat[1][0] = 2. * (xy - zw);
        mat[2][0] = 2. * (xz + yw);
        mat[0][1] = 2. * (xy + zw);
        mat[2][1] = 2. * (yz - xw);
        mat[0][2] = 2. * (xz - yw);
        mat[1][2] = 2. * (yz + xw);

        mat[3][3] = 1.;

        return mat;
    }

    Quaternion inverse()
    {
        Quaternion ret    = conjugate();
        float      normsq = normSquared();

        ret.scalar /= normsq;
        ret.vector /= normsq;

        return ret;
    }

    float   scalar;
    Vector3 vector;
};

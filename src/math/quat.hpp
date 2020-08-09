#pragma once

#include "vec3.hpp"
#include <math.h>

struct Quaternion {

    Quaternion()
        : scalar(0.)
    {
    }

    Quaternion(double radian, vec3 vec)
        : scalar(cos(radian / 2))
        , vector(vec)
    {
        vec.normalize();
        vec *= sin(radian / 2);
    }

    void operator+=(Quaternion& q)
    {
        scalar += q.scalar;
        vector += q.vector;
    }

    Quaternion operator+(Quaternion& q)
    {
        Quaternion ret;

        ret.scalar = scalar + q.scalar;
        ret.vector = vector + q.vector;

        return ret;
    }

    void operator-=(Quaternion& q)
    {
        scalar -= q.scalar;
        vector -= q.vector;
    }

    Quaternion operator-(Quaternion& q)
    {
        Quaternion ret;

        ret.scalar = scalar - q.scalar;
        ret.vector = vector - q.vector;

        return ret;
    }

    Quaternion operator*(Quaternion& q)
    {
        Quaternion ret;

        ret.scalar = scalar * q.scalar - vector.dot(q.vector);
        ret.vector = (q.vector * scalar + vector * q.scalar) + vector.cross(q.vector);

        return ret;
    }

    void operator*=(Quaternion& q)
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

    double normSquared()
    {
        return scalar * scalar + vector.normSquared();
    }

    double norm()
    {
        return sqrt(scalar * scalar + vector.normSquared());
    }

    Quaternion inverse()
    {
        Quaternion ret    = conjugate();
        double     normsq = normSquared();

        ret.scalar /= normsq;
        ret.vector /= normsq;

        return ret;
    }

    double scalar;
    vec3   vector;
};

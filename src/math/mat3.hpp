#pragma once

#include "vec3.hpp"

struct Matrix33 {

    Matrix33() {};
    Matrix33(Vector3 a, Vector3 b, Vector3 c)
    {
        mat[0] = a;
        mat[1] = b;
        mat[2] = c;
    };

    Matrix33(float diag_val)
    {
        mat[0][1] = 0.;
        mat[0][2] = 0.;
        mat[1][0] = 0.;
        mat[1][2] = 0.;
        mat[2][0] = 0.;
        mat[2][1] = 0.;

        mat[0][0] = diag_val;
        mat[1][1] = diag_val;
        mat[2][2] = diag_val;
    }

    Vector3& operator[](int index)
    {
        return mat[index];
    }

    const Vector3& operator[](int index) const
    {
        return mat[index];
    }

    Matrix33 operator*(const Matrix33& b)
    {

        Matrix33 ret;

        ret[0][0] = mat[0][0] * b[0][0] + mat[1][0] * b[0][1] + mat[2][0] * b[0][2];
        ret[0][1] = mat[0][1] * b[0][0] + mat[1][1] * b[0][1] + mat[2][1] * b[0][2];
        ret[0][2] = mat[0][2] * b[0][0] + mat[1][2] * b[0][1] + mat[2][2] * b[0][2];

        ret[1][0] = mat[0][0] * b[1][0] + mat[1][0] * b[1][1] + mat[2][0] * b[1][2];
        ret[1][1] = mat[0][1] * b[1][0] + mat[1][1] * b[1][1] + mat[2][1] * b[1][2];
        ret[1][2] = mat[0][2] * b[1][0] + mat[1][2] * b[1][1] + mat[2][2] * b[1][2];

        ret[2][0] = mat[0][0] * b[2][0] + mat[1][0] * b[2][1] + mat[2][0] * b[2][2];
        ret[2][1] = mat[0][1] * b[2][0] + mat[1][1] * b[2][1] + mat[2][1] * b[2][2];
        ret[2][2] = mat[0][2] * b[2][0] + mat[1][2] * b[2][1] + mat[2][2] * b[2][2];

        return ret;
    }

    Vector3 operator*(const Vector3& b)
    {
        Vector3 ret;

        ret[0] = b[0] * mat[0][0] + b[1] * mat[1][0] + b[2] * mat[2][0];
        ret[1] = b[0] * mat[0][1] + b[1] * mat[1][1] + b[2] * mat[2][1];
        ret[2] = b[0] * mat[0][2] + b[1] * mat[1][2] + b[2] * mat[2][2];

        return ret;
    }

    float determinant()
    {
        float m00 = mat[0][0];
        float m01 = mat[0][1];
        float m02 = mat[0][2];
        float m10 = mat[1][0];
        float m11 = mat[1][1];
        float m12 = mat[1][2];
        float m20 = mat[2][0];
        float m21 = mat[2][1];
        float m22 = mat[2][2];

        return m00 * (m11 * m22 - m21 * m12) - m10 * (m01 * m22 - m02 * m21) + m20 * (m01 * m12 - m02 * m11);
    }

    void invert()
    {
        float m00 = mat[0][0];
        float m01 = mat[0][1];
        float m02 = mat[0][2];
        float m10 = mat[1][0];
        float m11 = mat[1][1];
        float m12 = mat[1][2];
        float m20 = mat[2][0];
        float m21 = mat[2][1];
        float m22 = mat[2][2];

        mat[0][0] = m11 * m22 - m12 * m21;
        mat[0][1] = -(m01 * m22 - m21 * m02);
        mat[0][2] = m01 * m12 - m11 * m02;
        mat[1][0] = -(m10 * m22 - m20 * m12);
        mat[1][1] = m00 * m22 - m02 * m20;
        mat[1][2] = -(m00 * m12 - m10 * m02);
        mat[2][0] = m10 * m21 - m20 * m11;
        mat[2][1] = -(m00 * m21 - m20 * m01);
        mat[2][2] = m00 * m11 - m01 * m10;

        float div = 1. / (m00 * mat[0][0] + m01 * mat[1][0] + m02 * mat[2][0]);

        scale(div);
    }

    void transpose()
    {
        Matrix33 tmp;

        tmp[0][1] = mat[1][0];
        tmp[0][2] = mat[2][0];
        tmp[1][0] = mat[0][1];
        tmp[1][2] = mat[2][1];
        tmp[2][0] = mat[0][2];
        tmp[2][1] = mat[1][2];

        mat[0][1] = tmp[0][1];
        mat[0][2] = tmp[0][2];
        mat[1][0] = tmp[1][0];
        mat[1][2] = tmp[1][2];
        mat[2][0] = tmp[2][0];
        mat[2][1] = tmp[2][1];
    }

    void colswap(unsigned int c1, unsigned int c2)
    {
        Vector3 tmprow;

        tmprow  = mat[c1];
        mat[c1] = mat[c2];
        mat[c2] = tmprow;
    }

    void rowswap(unsigned int r1, unsigned int r2)
    {
        Vector3 tmpcol;

        tmpcol[0] = mat[0][r1];
        tmpcol[1] = mat[1][r1];
        tmpcol[2] = mat[2][r1];

        mat[0][r1] = mat[0][r2];
        mat[1][r1] = mat[1][r2];
        mat[2][r1] = mat[2][r2];

        mat[0][r2] = tmpcol[0];
        mat[1][r2] = tmpcol[1];
        mat[2][r2] = tmpcol[2];
    }

    void scale(float val)
    {
        mat[0][0] *= val;
        mat[0][1] *= val;
        mat[0][2] *= val;
        mat[1][0] *= val;
        mat[1][1] *= val;
        mat[1][2] *= val;
        mat[2][0] *= val;
        mat[2][1] *= val;
        mat[2][2] *= val;
    }

    Vector3 mat[3];
};

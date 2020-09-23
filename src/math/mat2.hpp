#pragma once
#include "vec2.hpp"

struct Matrix22 {

    Matrix22() {};

    Matrix22(Vector2 a, Vector2 b)
    {
        mat[0] = a;
        mat[1] = b;
    };

    Matrix22(float diag_val)
    {
        mat[0][1] = 0.;
        mat[1][0] = 0.;

        mat[0][0] = diag_val;
        mat[1][1] = diag_val;
    }

    Vector2& operator[](int index)
    {
        return mat[index];
    }

    const Vector2& operator[](int index) const
    {
        return mat[index];
    }

    Matrix22 operator*(const Matrix22& b)
    {
        Matrix22 ret;

        ret[0][0] = mat[0][0] * b[0][0] + mat[1][0] * b[0][1];
        ret[0][1] = mat[0][1] * b[0][0] + mat[1][1] * b[0][1];
        ret[1][0] = mat[0][0] * b[1][0] + mat[1][0] * b[1][1];
        ret[1][1] = mat[0][1] * b[1][0] + mat[1][1] * b[1][1];

        return ret;
    }

    Vector2 operator*(const Vector2& b)
    {
        Vector2 ret;

        ret[0] = b[0] * mat[0][0] + b[1] * mat[1][0];
        ret[1] = b[0] * mat[0][1] + b[1] * mat[1][1];

        return ret;
    }

    float determinant()
    {
        return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
    }

    void invert()
    {
        float det = determinant();
        float tmp = mat[0][0];

        mat[0][0] = mat[1][1] / det;
        mat[0][1] = (-mat[0][1]) / det;
        mat[1][0] = (-mat[1][0]) / det;
        mat[1][1] = tmp / det;
    }

    void transpose()
    {
        float tmp = mat[1][0];

        mat[1][0] = mat[0][1];
        mat[0][1] = tmp;
    }

    void scale(float val)
    {
        mat[0][0] *= val;
        mat[0][1] *= val;
        mat[1][0] *= val;
        mat[1][1] *= val;
    }

    void colswap(unsigned int c1, unsigned int c2)
    {
        Vector2 tmprow;

        tmprow = mat[c1];

        mat[c1] = mat[c2];
        mat[c2] = tmprow;
    }

    void rowswap(unsigned int r1, unsigned int r2)
    {

        Vector2 tmpcol;

        tmpcol[0] = mat[0][r1];
        tmpcol[1] = mat[1][r1];

        mat[0][r1] = mat[0][r2];
        mat[1][r1] = mat[1][r2];
        mat[0][r2] = tmpcol[0];
        mat[1][r2] = tmpcol[1];
    }

    Vector2 mat[2];
};

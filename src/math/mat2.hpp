#pragma once
#include "vec2.hpp"

struct mat2 {

    mat2() {};

    mat2(vec2 a, vec2 b)
    {
        mat[0] = a;
        mat[1] = b;
    };

    mat2(double diag_val)
    {
        mat[0][1] = 0.;
        mat[1][0] = 0.;

        mat[0][0] = diag_val;
        mat[1][1] = diag_val;
    }

    vec2& operator[](int index)
    {
        return mat[index];
    }

    const vec2& operator[](int index) const
    {
        return mat[index];
    }

    mat2 operator*(const mat2& b)
    {
        mat2 ret;

        ret[0][0] = mat[0][0] * b[0][0] + mat[0][1] * b[1][0];
        ret[0][1] = mat[0][0] * b[1][0] + mat[0][1] * b[1][1];
        ret[1][0] = mat[1][0] * b[0][0] + mat[1][1] * b[1][0];
        ret[0][1] = mat[1][0] * b[1][0] + mat[1][1] * b[1][1];

        return ret;
    }

    vec2 operator*(const vec2& b)
    {
        vec2 ret;

        ret[0] = b[0] * mat[0][0] + b[1] * mat[1][0];
        ret[1] = b[0] * mat[0][1] + b[1] * mat[1][1];

        return ret;
    }

    double determinant()
    {
        return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
    }

    void invert()
    {
        double det = determinant();
        double tmp = mat[0][0];

        mat[1][0] = (-mat[1][0]) / det;
        mat[0][1] = (-mat[0][1]) / det;
        mat[0][0] = mat[1][1] / det;
        mat[1][1] = tmp / det;
    }

    void transpose()
    {
        double tmp = mat[1][0];

        mat[1][0] = mat[0][1];
        mat[0][1] = tmp;
    }

    void scale(double val)
    {
        mat[0][0] *= val;
        mat[0][1] *= val;
        mat[1][0] *= val;
        mat[1][1] *= val;
    }

    void colswap(unsigned int c1, unsigned int c2)
    {
        vec2 tmpcol;

        tmpcol[0] = mat[0][c1];
        tmpcol[1] = mat[1][c1];

        mat[0][c1] = mat[0][c2];
        mat[1][c1] = mat[1][c2];
        mat[0][c2] = tmpcol[0];
        mat[1][c2] = tmpcol[1];
    }

    void rowswap(unsigned int r1, unsigned int r2)
    {
        vec2 tmprow;

        tmprow = mat[r1];

        mat[r1] = mat[r2];
        mat[r2] = tmprow;
    }

    vec2 mat[2];
};

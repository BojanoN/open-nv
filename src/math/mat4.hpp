#pragma once
#include "vec4.hpp"
#include <initializer_list>
struct mat4 {

    mat4() {};

    mat4(vec4 a, vec4 b, vec4 c, vec4 d)
    {
        mat[0] = a;
        mat[1] = b;
        mat[2] = c;
        mat[3] = d;
    };

    mat4(double diag_val)
    {
        mat[0][1] = 0.;
        mat[0][2] = 0.;
        mat[0][3] = 0.;
        mat[1][0] = 0.;
        mat[1][2] = 0.;
        mat[1][3] = 0.;
        mat[2][0] = 0.;
        mat[2][1] = 0.;
        mat[2][3] = 0.;
        mat[3][0] = 0.;
        mat[3][1] = 0.;
        mat[3][2] = 0.;

        mat[0][0] = diag_val;
        mat[1][1] = diag_val;
        mat[2][2] = diag_val;
        mat[3][3] = diag_val;
    }

    vec4& operator[](int index)
    {
        return mat[index];
    }

    void scale(double val)
    {
        mat[0][0] *= val;
        mat[0][1] *= val;
        mat[0][2] *= val;
        mat[0][3] *= val;
        mat[1][0] *= val;
        mat[1][1] *= val;
        mat[1][2] *= val;
        mat[1][3] *= val;
        mat[2][0] *= val;
        mat[2][1] *= val;
        mat[2][2] *= val;
        mat[2][3] *= val;
        mat[3][0] *= val;
        mat[3][1] *= val;
        mat[3][2] *= val;
        mat[3][3] *= val;
    }

    mat4 operator*(mat4& b)
    {
        mat4 ret;

        ret[0][0] = mat[0][0] * b[0][0] + mat[0][1] * b[1][0] + mat[0][2] * b[2][0] + mat[0][3] * b[3][0];
        ret[0][1] = mat[0][0] * b[0][1] + mat[0][1] * b[1][1] + mat[0][2] * b[2][1] + mat[0][3] * b[3][1];
        ret[0][2] = mat[0][0] * b[0][2] + mat[0][1] * b[1][2] + mat[0][2] * b[2][2] + mat[0][3] * b[3][2];
        ret[0][3] = mat[0][0] * b[0][3] + mat[0][1] * b[1][3] + mat[0][2] * b[2][3] + mat[0][3] * b[3][3];

        ret[1][0] = mat[1][0] * b[0][0] + mat[1][1] * b[1][0] + mat[1][2] * b[2][0] + mat[1][3] * b[3][0];
        ret[1][1] = mat[1][0] * b[0][1] + mat[1][1] * b[1][1] + mat[1][2] * b[2][1] + mat[1][3] * b[3][1];
        ret[1][2] = mat[1][0] * b[0][2] + mat[1][1] * b[1][2] + mat[1][2] * b[2][2] + mat[1][3] * b[3][2];
        ret[1][3] = mat[1][0] * b[0][2] + mat[1][1] * b[1][2] + mat[1][2] * b[2][3] + mat[1][3] * b[3][3];

        ret[2][0] = mat[2][0] * b[0][0] + mat[2][1] * b[1][0] + mat[2][2] * b[2][0] + mat[2][3] * b[3][0];
        ret[2][1] = mat[2][0] * b[0][1] + mat[2][1] * b[1][1] + mat[2][2] * b[2][1] + mat[2][3] * b[3][1];
        ret[2][2] = mat[2][0] * b[0][2] + mat[2][1] * b[1][2] + mat[2][2] * b[2][2] + mat[2][3] * b[3][2];
        ret[2][3] = mat[2][0] * b[0][3] + mat[2][1] * b[1][3] + mat[2][2] * b[2][3] + mat[2][3] * b[3][3];

        ret[3][0] = mat[3][0] * b[0][0] + mat[3][1] * b[1][0] + mat[3][2] * b[2][0] + mat[3][3] * b[3][0];
        ret[3][1] = mat[3][0] * b[0][1] + mat[3][1] * b[1][1] + mat[3][2] * b[2][1] + mat[3][3] * b[3][1];
        ret[3][2] = mat[3][0] * b[0][2] + mat[3][1] * b[1][2] + mat[3][2] * b[2][2] + mat[3][3] * b[3][2];
        ret[3][3] = mat[3][0] * b[0][3] + mat[3][1] * b[1][3] + mat[3][2] * b[2][3] + mat[3][3] * b[3][3];

        return ret;
    }

    double determinant()
    {
        double m00 = mat[0][0];
        double m01 = mat[0][1];
        double m02 = mat[0][2];
        double m03 = mat[0][3];
        double m10 = mat[1][0];
        double m11 = mat[1][1];
        double m12 = mat[1][2];
        double m13 = mat[1][3];
        double m20 = mat[2][0];
        double m21 = mat[2][1];
        double m22 = mat[2][2];
        double m23 = mat[2][3];
        double m30 = mat[3][0];
        double m31 = mat[3][1];
        double m32 = mat[3][2];
        double m33 = mat[3][3];

        double min1 = m22 * m33 - m32 * m23;
        double min2 = m21 * m33 - m31 * m23;
        double min3 = m21 * m32 - m31 * m22;
        double min4 = m20 * m33 - m30 * m23;
        double min5 = m20 * m32 - m30 * m22;
        double min6 = m20 * m31 - m30 * m21;

        return m00 * (m11 * min1 - m12 * min2 + m13 * min3)
            - m01 * (m10 * min1 - m12 * min4 + m13 * min5)
            + m02 * (m10 * min2 - m11 * min4 + m13 * min6)
            - m03 * (m10 * min3 - m11 * min5 + m12 * min6);
    }

    void invert()
    {
        double m00 = mat[0][0];
        double m01 = mat[0][1];
        double m02 = mat[0][2];
        double m03 = mat[0][3];
        double m10 = mat[1][0];
        double m11 = mat[1][1];
        double m12 = mat[1][2];
        double m13 = mat[1][3];
        double m20 = mat[2][0];
        double m21 = mat[2][1];
        double m22 = mat[2][2];
        double m23 = mat[2][3];
        double m30 = mat[3][0];
        double m31 = mat[3][1];
        double m32 = mat[3][2];
        double m33 = mat[3][3];

        double min1 = m22 * m33 - m32 * m23;
        double min2 = m21 * m33 - m31 * m23;
        double min3 = m21 * m32 - m31 * m22;
        double min4 = m20 * m33 - m30 * m23;
        double min5 = m20 * m32 - m30 * m22;
        double min6 = m20 * m31 - m30 * m21;

        mat[0][0] = m11 * min1 - m12 * min2 + m13 * min3;
        mat[1][0] = -(m10 * min1 - m12 * min4 + m13 * min5);
        mat[2][0] = m10 * min2 - m11 * min4 + m13 * min6;
        mat[3][0] = -(m10 * min3 - m11 * min5 + m12 * min6);

        mat[0][1] = -(m01 * min1 - m02 * min2 + m03 * min3);
        mat[1][1] = m00 * min1 - m02 * min4 + m03 * min5;
        mat[2][1] = -(m00 * min2 - m01 * min4 + m03 * min6);
        mat[3][1] = m00 * min3 - m01 * min5 + m02 * min6;

        min1 = m12 * m33 - m32 * m13;
        min2 = m11 * m33 - m31 * m13;
        min3 = m11 * m32 - m31 * m12;
        min4 = m10 * m33 - m30 * m13;
        min5 = m10 * m32 - m30 * m12;
        min6 = m10 * m31 - m30 * m11;

        mat[0][2] = m01 * min1 - m02 * min2 + m03 * min3;
        mat[1][2] = -(m00 * min1 - m02 * min4 + m03 * min5);
        mat[2][2] = m00 * min2 - m01 * min4 + m03 * min6;
        mat[3][2] = -(m00 * min3 - m01 * min5 + m02 * min6);

        min1 = m12 * m23 - m22 * m13;
        min2 = m11 * m23 - m21 * m13;
        min3 = m11 * m22 - m21 * m12;
        min4 = m10 * m23 - m20 * m13;
        min5 = m10 * m22 - m20 * m12;
        min6 = m10 * m21 - m20 * m11;

        mat[0][3] = -(m01 * min1 - m02 * min2 + m03 * min3);
        mat[1][3] = m00 * min1 - m02 * min4 + m03 * min5;
        mat[2][3] = -(m00 * min2 - m01 * min4 + m03 * min6);
        mat[3][3] = m00 * min3 - m01 * min5 + m02 * min6;

        double det = 1. / (m00 * mat[0][0] + m01 * mat[1][0] + m02 * mat[2][0] + m03 * mat[3][0]);

        scale(det);
    }

    void colswap(unsigned int c1, unsigned int c2)
    {
        vec4 tmpcol;

        tmpcol[0] = mat[0][c1];
        tmpcol[1] = mat[1][c1];
        tmpcol[2] = mat[2][c1];
        tmpcol[3] = mat[3][c1];

        mat[0][c1] = mat[0][c2];
        mat[1][c1] = mat[1][c2];
        mat[2][c1] = mat[2][c2];
        mat[3][c1] = mat[3][c2];

        mat[0][c2] = tmpcol[0];
        mat[1][c2] = tmpcol[1];
        mat[2][c2] = tmpcol[2];
        mat[3][c2] = tmpcol[3];
    }

    void rowswap(unsigned int r1, unsigned int r2)
    {
        vec4 tmprow;

        tmprow  = mat[r1];
        mat[r1] = mat[r2];
        mat[r2] = tmprow;
    }

    vec4 mat[4];
};

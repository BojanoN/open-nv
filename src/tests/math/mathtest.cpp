#include "mat2.hpp"
#include "mat3.hpp"
#include "mat4.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "matutil.hpp"

#include <assert.h>
#include <math.h>

#define ASSERT_EQUALS_DOUBLE(val1, val2) assert(abs((val1) - (val2)) < 10E-7)

int main(void)
{
    Vector2 v2 = { 3., 4. };
    Vector3 v3 = { 1., 12., 6. };
    Vector4 v4 = { 13., 5., 9., 11. };

    Matrix22 m2 = { { 1., 2. }, { 7., 4. } };

    Matrix22 m2inv = {
        { -2. / 5., 1. / 5. },
        { 7. / 10., -1. / 10. }
    };

    Matrix33 m3 = {
        { 3., 2., 1. },
        { 4., 5., 6. },
        { 9., 8., 11. }
    };

    Matrix33 m3inv = {
        { 1. / 4., -1. / 2., 1. / 4. },
        { 5. / 14., 6. / 7., -1. / 2. },
        { -13. / 28., -3. / 14, 1. / 4. }
    };

    Matrix44 m4 {
        { 3., 2., 1., 9. },
        { 4., 5., 6., 13. },
        { 9., 8., 11., 13. },
        { 3., 1., 12., 1. }
    };

    Matrix44 m4inv = {
        { 41. / 114., -8. / 19., 97. / 570., 7. / 285. },
        { -115. / 228., 9. / 38., 29. / 228., -11. / 57. },
        { -13. / 228., 3. / 38., -53. / 1140., 26. / 285. },
        { 25. / 228., 3. / 38., -91. / 1140., 7. / 285. }
    };

    Matrix22 m2ident(1.);
    Matrix33 m3ident(1.);
    Matrix44 m4ident(1.);

    ASSERT_EQUALS_DOUBLE(5., v2.norm());
    ASSERT_EQUALS_DOUBLE(13.45362404707371, v3.norm());
    ASSERT_EQUALS_DOUBLE(19.899748, v4.norm());

    ASSERT_EQUALS_DOUBLE(-10., m2.determinant());
    ASSERT_EQUALS_DOUBLE(28., m3.determinant());
    ASSERT_EQUALS_DOUBLE(1140., m4.determinant());

    assert(mat2_equal(m2 * m2inv, m2ident));
    assert(mat3_equal(m3 * m3inv, m3ident));
    assert(mat4_equal(m4 * m4inv, m4ident));

    m2.invert();
    m3.invert();
    m4.invert();

    assert(mat2_equal(m2, m2inv));
    assert(mat3_equal(m3, m3inv));
    assert(mat4_equal(m4, m4inv));

    Matrix44 t4 {
        { 3., 2., 1., 9. },
        { 4., 5., 6., 13. },
        { 9., 8., 11., 13. },
        { 3., 1., 12., 1. }
    };

    Matrix44 p4 {
        { 1., 2., 3., 4. },
        { 4., 5., 6., 7. },
        { 8., 9., 10., 11. },
        { 12., 13., 14., 15. }
    };

    Matrix44 res {
        { 50., 40., 94., 78. },
        { 107., 88., 184., 186. },
        { 183., 152., 304., 330. },
        { 259., 216., 424., 474. }
    };

    assert(mat4_equal(t4 * p4, res));

    // Util tests
    Vector3 pos { 0., 0., 0. };
    Vector3 center { 4., 4., 4. }; // pos - (0,0,-1)
    Vector3 x { 0., 1., 0. };

    Matrix44 look {
        { -0.707107, -0.408248, -0.577350, 0. },
        { 0., 0.816497, -0.577350, 0. },
        { 0.707107, -0.408248, -0.577350, 0. },
        { 0., 0., 0., 1. }
    };

    Matrix44 ourLook = lookat_mat(pos, center, x);

    assert(mat4_equal(look, ourLook));

    return 0;
}

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
    vec2 v2 = { 3., 4. };
    vec3 v3 = { 1., 12., 6. };
    vec4 v4 = { 13., 5., 9., 11. };

    mat2 m2 = { { 1., 2. }, { 7., 4. } };

    mat2 m2inv = {
        { -2. / 5., 1. / 5. },
        { 7. / 10., -1. / 10. }
    };

    mat3 m3 = {
        { 3., 2., 1. },
        { 4., 5., 6. },
        { 9., 8., 11. }
    };

    mat3 m3inv = {
        { 1. / 4., -1. / 2., 1. / 4. },
        { 5. / 14., 6. / 7., -1. / 2. },
        { -13. / 28., -3. / 14, 1. / 4. }
    };

    mat4 m4 { { 3., 2., 1., 9. },
        { 4., 5., 6., 13. },
        { 9., 8., 11., 13. },
        { 3., 1., 12., 1. } };

    mat4 m4inv = {
        { 41. / 114., -8. / 19., 97. / 570., 7. / 285. },
        { -115. / 228., 9. / 38., 29. / 228., -11. / 57. },
        { -13. / 228., 3. / 38., -53. / 1140., 26. / 285. },
        { 25. / 228., 3. / 38., -91. / 1140., 7. / 285. }
    };

    ASSERT_EQUALS_DOUBLE(5., v2.norm());
    ASSERT_EQUALS_DOUBLE(13.45362404707371, v3.norm());
    ASSERT_EQUALS_DOUBLE(19.899748, v4.norm());

    ASSERT_EQUALS_DOUBLE(-10., m2.determinant());
    ASSERT_EQUALS_DOUBLE(28., m3.determinant());
    ASSERT_EQUALS_DOUBLE(1140., m4.determinant());

    m2.invert();
    m3.invert();
    m4.invert();

    assert(mat2_equal(m2, m2inv));
    assert(mat3_equal(m3, m3inv));
    assert(mat4_equal(m4, m4inv));

    return 0;
}

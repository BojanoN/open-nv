#include "types.hpp"
#include "vec2.hpp"

int main(void)
{
    vec2 t = { 0., 1. };
    vec2 r = { 1., 1. };
    vec2 y;

    vec2_add(t, r, y);

    vec2_print(y);

    return 0;
}

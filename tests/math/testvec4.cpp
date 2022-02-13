#include <gtest/gtest.h>
#include "math/vector4.h"

using namespace noble_steed;

TEST(Vec4, Floor) {
    vec4 rnd = {0.445f, 0.557f, -4.56, 3.14};
    EXPECT_EQ(math::floor(rnd), vec4(0, 0, -5, 3));
    rnd = {-34.445123f, -0.5888899f, 6.45, 3456.99};
    EXPECT_EQ(math::floor(rnd), vec4(-35,-1, 6, 3456));
    rnd = {34.445123f, 9.5888899f, -55.55, -12345.11};
    EXPECT_EQ(math::floor(rnd), vec4(34, 9, -56, -12346));
}
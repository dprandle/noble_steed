#include "math/nsvec2.h"
#include <gtest/gtest.h>

using namespace noble_steed;

TEST(Vec2, Project) { 
    vec2 v2 {0.5, -0.8};
    vec2 v2_2 {44.0, 51.0};
    auto v3 = project(v2, v2_2);
    vec2 v4 {-0.18232, -0.21133};
    vec2 eps{0.001, 0.001};
    EXPECT_EQ(v3, v4);
    v3 = project(v2_2, v2);
    v4 = {-10.562, 16.900};
    EXPECT_EQ(v3, v4);
}
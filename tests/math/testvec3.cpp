#include "math/vector3.h"
#include <gtest/gtest.h>

using namespace noble_steed;

TEST(Vec3, Project) { 
    vec3 v2 {0.5, -0.8, 1.2};
    vec3 v2_2 {-2.1, 1.5, -3.1};
    auto v3 = math::project(v2, v2_2);
    vec3 v4 {12537.0/16270.0, -1791.0/3254.0, 18507.0/16270.0};
    EXPECT_EQ(v3, v4);
    v3 = math::project(v2_2, v2);
    v4 = {-597.0/466.0, 2388.0/1165.0, -3582.0/1165.0};
    EXPECT_EQ(v3, v4);

    ivec3 v2i {2, -6, 5};
    ivec3 v2_2i {-22, 55, 24};
    EXPECT_EQ(math::project(v2i, v2_2i), ivec3(1, -3, -1));
}

TEST(Vec3, Angle) {
    vec3 v1 {0,0,1};
    vec3 v2 {1,0,0};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 90.0f * math::TO_RADS);
    v2 = {0, 1, 0};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 90.0f * math::TO_RADS);
    v2 = {0, 0, 7};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 0.0f * math::TO_RADS);
    v2 = {3.33, 1.87, 0};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 90.0f * math::TO_RADS);
    v2 = {0, 1, 1};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 45.0f * math::TO_RADS);
    v2 = {1, 0, 1};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 45.0f * math::TO_RADS);
    v1 = {4.3, 1.22, 3.64};
    v2 = {-2.5, 1.89, 9.64};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 62.870 * math::TO_RADS);
}

TEST(Vec3, MinMax) {
    vec3 v1{3,-6, 5};
    vec3 v2{4, 4, -2};
    vec3 v3{-8, 9, 3};
    vec3 v4{-5, -3, 0};
    EXPECT_EQ(math::minimums(v1, v1), vec3(3, -6, 5));
    EXPECT_EQ(math::maximums(v1, v1), vec3(3, -6, 5));

    EXPECT_EQ(math::minimums(v1, v2), vec3(3, -6, -2));
    EXPECT_EQ(math::maximums(v1, v2), vec3(4, 4, 5));
    EXPECT_EQ(math::minimums(v2, v1), vec3(3, -6, -2));
    EXPECT_EQ(math::maximums(v2, v1), vec3(4, 4, 5));

    EXPECT_EQ(math::minimums(v1, v3), vec3(-8, -6, 3));
    EXPECT_EQ(math::maximums(v1, v3), vec3(3, 9, 5));
    EXPECT_EQ(math::minimums(v3, v1), vec3(-8, -6, 3));
    EXPECT_EQ(math::maximums(v3, v1), vec3(3, 9, 5));

    EXPECT_EQ(math::minimums(v1, v4), vec3(-5, -6, 0));
    EXPECT_EQ(math::maximums(v1, v4), vec3(3, -3, 5));
    EXPECT_EQ(math::minimums(v4, v1), vec3(-5, -6, 0));
    EXPECT_EQ(math::maximums(v4, v1), vec3(3, -3, 5));

    EXPECT_EQ(math::minimums(v2, v3), vec3(-8, 4, -2));
    EXPECT_EQ(math::maximums(v2, v3), vec3(4, 9, 3));
    EXPECT_EQ(math::minimums(v3, v2), vec3(-8, 4, -2));
    EXPECT_EQ(math::maximums(v3, v2), vec3(4, 9, 3));

    EXPECT_EQ(math::minimums(v2, v4), vec3(-5, -3, -2));
    EXPECT_EQ(math::maximums(v2, v4), vec3(4, 4, 0));
    EXPECT_EQ(math::minimums(v4, v2), vec3(-5, -3, -2));
    EXPECT_EQ(math::maximums(v4, v2), vec3(4, 4, 0));

    EXPECT_EQ(math::minimums(v3, v4), vec3(-8, -3, 0));
    EXPECT_EQ(math::maximums(v3, v4), vec3(-5, 9, 3));
    EXPECT_EQ(math::minimums(v4, v3), vec3(-8, -3, 0));
    EXPECT_EQ(math::maximums(v4, v3), vec3(-5, 9, 3));
}

TEST(Vec3, Ceil) {
    vec3 rnd = {0.445f, 0.557f, -4.56};
    EXPECT_EQ(math::ceil(rnd), vec3(1,1,-4));
    rnd = {-34.445123f, -0.5888899f, 6.45};
    EXPECT_EQ(math::ceil(rnd), vec3(-34,0,7));
    rnd = {34.445123f, 9.5888899f, -55.55};
    EXPECT_EQ(math::ceil(rnd), vec3(35,10, -55));
}

TEST(Vec3, Floor) {
    vec3 rnd = {0.445f, 0.557f, -4.56};
    EXPECT_EQ(math::floor(rnd), vec3(0,0,-5));
    rnd = {-34.445123f, -0.5888899f, 6.45};
    EXPECT_EQ(math::floor(rnd), vec3(-35,-1,6));
    rnd = {34.445123f, 9.5888899f, -55.55};
    EXPECT_EQ(math::floor(rnd), vec3(34, 9, -56));
}

TEST(Vec3, Round) {
    vec3 rnd = {0.445f, 0.557f, -4.56};
    EXPECT_EQ(math::round(rnd), vec3(0, 1,-5));
    EXPECT_EQ(math::round(rnd, 2), vec3(0.44, 0.56,-4.56));
    rnd = {-34.445123f, -0.5888899f, 6.45};
    EXPECT_EQ(math::round(rnd), vec3(-34, -1, 6));
    EXPECT_EQ(math::round(rnd, 4), vec3(-34.4451, -0.5889, 6.45));
    EXPECT_EQ(math::round(rnd, 3), vec3(-34.445, -0.589, 6.45));
}

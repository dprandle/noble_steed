#include "math/vector2.h"
#include "math/vector3.h"
#include "container/vector.h"
#include <iostream>
#include <gtest/gtest.h>

using namespace noble_steed;

TEST(Vec2, Project) { 
    vec2 v2 {0.5, -0.8};
    vec2 v2_2 {44.0, 51.0};
    auto v3 = math::project(v2, v2_2);
    vec2 v4 {-0.18232, -0.21133};
    EXPECT_EQ(v3, v4);
    v3 = math::project(v2_2, v2);
    v4 = {-940.0 / 89.0, 1504.0 / 89.0};
    EXPECT_EQ(v3, v4);

    ivec2 v2i {2, -6};
    ivec2 v2_2i {44, 51};
    EXPECT_EQ(math::project(v2i, v2_2i), ivec2(-2, -2));
}

TEST(Vec2, Angle) {
    vec2 v1 {1,1};
    vec2 v2 {1,0};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 45.0f * math::TO_RADS);
    v1 = {1, 0};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 0.0f);
    v1 = {0, 1};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 90.0f * math::TO_RADS);
    v1 = {-1, 1};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 135.0f * math::TO_RADS);
    v1 = {-1, 0};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 180.0f * math::TO_RADS);
    v1 = {-1, -1};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 135.0f * math::TO_RADS);
    v1 = {0, -1};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 90.0f * math::TO_RADS);
    v1 = {1, -1};
    EXPECT_FLOAT_EQ(math::angle(v1, v2), 45.0f * math::TO_RADS);
}

TEST(Vec2, Distance) {
    vec2 v1 {45.777,math::PI};
    vec2 v2 {-3.33,108.22};
    vec2 v3 {21.1,0.005f};
    EXPECT_FLOAT_EQ(math::length(v2 - v1), 115.9869);
    EXPECT_FLOAT_EQ(math::length(v3 - v2), 110.9383);
    EXPECT_FLOAT_EQ(math::length(v3 - v1), 24.87554);
}

TEST(Vec2, Reflect) {
    vec2 v2{1,1};
    vec2 norm{1,1};
    EXPECT_EQ(math::reflect(v2, norm), vec2(-3,-3));
    v2 = {-1,0};
    EXPECT_EQ(math::reflect(v2, norm), vec2(1,2));
    v2 = {6,2};
    norm = {14,-13};
    EXPECT_EQ(math::reflect(v2, norm), vec2(-1618.00,1510.00));
}

TEST(Vec2, Normalize) {
    vec2 v2{};
    math::normalize(&v2);
    EXPECT_EQ(v2, vec2());
    v2 = {1,1};
    math::normalize(&v2);
    EXPECT_EQ(v2, vec2(0.5*math::sqrt(2.0)));
    EXPECT_FLOAT_EQ(math::length(v2), 1.0f);
    v2 = {-343.0f, 0.556f};
    math::normalize(&v2);
    EXPECT_FLOAT_EQ(math::length(v2), 1.0f);
    EXPECT_FLOAT_EQ(math::length_sq(v2), 1.0f);
    EXPECT_EQ(v2, (vec2{-0.999999f, 0.001621f}));
}

TEST(Vec2, LTGTOperators) {
    vec2 l(1, 2);
    vec2 r(3, 4);
    EXPECT_TRUE(l < r);
    l.x = 3.5;
    EXPECT_FALSE(l < r);
    r = {-2, -3};
    EXPECT_TRUE(r < l);
    l = {0.56789, 0.6678};
    r = {0.56789, 0.6678};
    EXPECT_TRUE(l == r);
    EXPECT_FALSE(l < r);
    EXPECT_FALSE(l > r);
    EXPECT_TRUE(l <= r);
    EXPECT_TRUE(l >= r);
    EXPECT_FALSE(l != r);
    r = {-2, -3};
    l = {0, -7};
    EXPECT_FALSE(l > r);
}

TEST(Vec2, Arithmatic_Operators) {
    vec2 v1 {0, 0};
    v1 *= 10;
    EXPECT_EQ(v1, vec2(0));
    v1 += vec2(10);
    EXPECT_EQ(v1, vec2(10));
    v1 *= vec2(2, -3);
    EXPECT_EQ(v1, vec2(20,-30));
    v1 /= vec2(2, -3);
    EXPECT_EQ(v1, vec2(10));
    v1 += vec2(3,6);
    EXPECT_EQ(v1, vec2(13,16));
    ++v1;
    EXPECT_EQ(v1, vec2(14,17));
    --v1;
    EXPECT_EQ(v1, vec2(13,16));
    v1 *= 2;
    EXPECT_EQ(v1, vec2(26,32));
    v1 /= 2;
    EXPECT_EQ(v1, vec2(13,16));
    EXPECT_EQ(v1 + vec2(5), vec2(18, 21));
    EXPECT_EQ(v1 - vec2(5), vec2(8,11));
    EXPECT_EQ(vec2(100, 100) / 10, vec2(10));
    EXPECT_EQ(100.0 / vec2(10, 10), vec2(10));
}


TEST(Vec2, MinMax) {
    vec2 v1{3,-6};
    vec2 v2{4, 4};
    vec2 v3{-8, 9};
    vec2 v4{-5, -3};
    EXPECT_EQ(math::minimums(v1, v1), vec2(3, -6));
    EXPECT_EQ(math::maximums(v1, v1), vec2(3, -6));

    EXPECT_EQ(math::minimums(v1, v2), vec2(3, -6));
    EXPECT_EQ(math::maximums(v1, v2), vec2(4, 4));
    EXPECT_EQ(math::minimums(v2, v1), vec2(3, -6));
    EXPECT_EQ(math::maximums(v2, v1), vec2(4, 4));

    EXPECT_EQ(math::minimums(v1, v3), vec2(-8, -6));
    EXPECT_EQ(math::maximums(v1, v3), vec2(3, 9));
    EXPECT_EQ(math::minimums(v3, v1), vec2(-8, -6));
    EXPECT_EQ(math::maximums(v3, v1), vec2(3, 9));

    EXPECT_EQ(math::minimums(v1, v4), vec2(-5, -6));
    EXPECT_EQ(math::maximums(v1, v4), vec2(3, -3));
    EXPECT_EQ(math::minimums(v4, v1), vec2(-5, -6));
    EXPECT_EQ(math::maximums(v4, v1), vec2(3, -3));

    EXPECT_EQ(math::minimums(v2, v3), vec2(-8, 4));
    EXPECT_EQ(math::maximums(v2, v3), vec2(4, 9));
    EXPECT_EQ(math::minimums(v3, v2), vec2(-8, 4));
    EXPECT_EQ(math::maximums(v3, v2), vec2(4, 9));

    EXPECT_EQ(math::minimums(v2, v4), vec2(-5, -3));
    EXPECT_EQ(math::maximums(v2, v4), vec2(4, 4));
    EXPECT_EQ(math::minimums(v4, v2), vec2(-5, -3));
    EXPECT_EQ(math::maximums(v4, v2), vec2(4, 4));

    EXPECT_EQ(math::minimums(v3, v4), vec2(-8, -3));
    EXPECT_EQ(math::maximums(v3, v4), vec2(-5, 9));
    EXPECT_EQ(math::minimums(v4, v3), vec2(-8, -3));
    EXPECT_EQ(math::maximums(v4, v3), vec2(-5, 9));

}

TEST(Vec2, Abs) {
    vec2 rnd = {7.5, 5.0};
    EXPECT_EQ(math::abs(rnd), vec2(7.5,5.0));
    rnd = {-7.5, 5.0};
    EXPECT_EQ(math::abs(rnd), vec2(7.5,5.0));
    rnd = {-7.5, -5.0};
    EXPECT_EQ(math::abs(rnd), vec2(7.5,5.0));
    rnd = {7.5, -5.0};
    double ret = math::abs(-6.0);
    math::abs(&ret);
    std::vector<int> bla, bla2;
    bla.resize(20, -6);
    bla2.resize(20, 6);
    math::abs(&bla);
    EXPECT_EQ(bla, bla2);
    EXPECT_FLOAT_EQ(ret, 6.0);
    EXPECT_EQ(math::abs(rnd), vec2(7.5,5.0));
    auto val = std::min_element(&rnd.data[0], &rnd.data[2]);
    EXPECT_FLOAT_EQ(*val, -5.0);
}

TEST(Vec2, MinMaxElement)
{
    vec2 v1(-4, 5);
    vec2 v2(4, -5);
    vec2 v3(4, 5);
    vec2 v4(-4, -5);
    EXPECT_FLOAT_EQ(math::min_element(v1), -4);
    EXPECT_FLOAT_EQ(math::min_element(v2), -5);
    EXPECT_FLOAT_EQ(math::min_element(v3), 4);
    EXPECT_FLOAT_EQ(math::min_element(v4), -5);
    EXPECT_FLOAT_EQ(math::max_element(v1), 5);
    EXPECT_FLOAT_EQ(math::max_element(v2), 4);
    EXPECT_FLOAT_EQ(math::max_element(v3), 5);
    EXPECT_FLOAT_EQ(math::max_element(v4), -4);

    EXPECT_EQ(math::min_element(math::convert_elements<i32>(v1)), -4);
    EXPECT_EQ(math::min_element(math::convert_elements<i32>(v2)), -5);
    EXPECT_EQ(math::min_element(math::convert_elements<i32>(v3)), 4);
    EXPECT_EQ(math::min_element(math::convert_elements<i32>(v4)), -5);
    EXPECT_EQ(math::max_element(math::convert_elements<i32>(v1)), 5);
    EXPECT_EQ(math::max_element(math::convert_elements<i32>(v2)), 4);
    EXPECT_EQ(math::max_element(math::convert_elements<i32>(v3)), 5);
    EXPECT_EQ(math::max_element(math::convert_elements<i32>(v4)), -4);
}

TEST(Vec2, Ceil) {
    vec2 rnd = {0.445f, 0.557f};
    EXPECT_EQ(math::ceil(rnd), vec2(1,1));
    rnd = {-34.445123f, -0.5888899f};
    EXPECT_EQ(math::ceil(rnd), vec2(-34,0));
    rnd = {34.445123f, 9.5888899f};
    EXPECT_EQ(math::ceil(rnd), vec2(35,10));
}

TEST(Vec2, Floor) {
    vec2 rnd = {0.445f, 0.557f};
    EXPECT_EQ(math::floor(rnd), vec2(0,0));
    rnd = {-34.445123f, -0.5888899f};
    EXPECT_EQ(math::floor(rnd), vec2(-35,-1));
    rnd = {34.445123f, 9.5888899f};
    EXPECT_EQ(math::floor(rnd), vec2(34,9));
}

TEST(Vec2, Round) {
    vec2 rnd = {0.445f, 0.557f};
    EXPECT_EQ(math::round(rnd), vec2(0,1));
    EXPECT_EQ(math::round(rnd, 2), vec2(0.44, 0.56));
    rnd = {-34.445123f, -0.5888899f};
    EXPECT_EQ(math::round(rnd), vec2(-34,-1));
    EXPECT_EQ(math::round(rnd, 4), vec2(-34.4451, -0.5889));
}

TEST(Vec2, PolarCartesian) {
    EXPECT_EQ(math::cartesian_to_polar(vec2(1,1)), vec2(math::sqrt(2.0), 45.0 * math::TO_RADS));
    EXPECT_EQ(math::polar_to_cartesian(vec2(math::sqrt(2.0), 45.0 * math::TO_RADS)), vec2(1.0, 1.0));
}
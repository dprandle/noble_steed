#include "math/vector2.h"
#include <gtest/gtest.h>

using namespace noble_steed;
using namespace noble_steed::math;

TEST(Vec2, Project) { 
    vec2 v2 {0.5, -0.8};
    vec2 v2_2 {44.0, 51.0};
    auto v3 = project(v2, v2_2);
    vec2 v4 {-0.18232, -0.21133};
    vec2 eps{0.001, 0.001};
    EXPECT_EQ(v3, v4);
    v3 = project(v2_2, v2);
    v4 = {-940.0 / 89.0, 1504.0 / 89.0};
    EXPECT_EQ(v3, v4);
}

TEST(Vec2, Angle) {
    vec2 v1 {1,1};
    vec2 v2 {1,0};
    EXPECT_FLOAT_EQ(angle(v1, v2), 45.0f * TO_RADS);
    v1 = {1, 0};
    EXPECT_FLOAT_EQ(angle(v1, v2), 0.0f);
    v1 = {0, 1};
    EXPECT_FLOAT_EQ(angle(v1, v2), 90.0f * TO_RADS);
    v1 = {-1, 1};
    EXPECT_FLOAT_EQ(angle(v1, v2), 135.0f * TO_RADS);
    v1 = {-1, 0};
    EXPECT_FLOAT_EQ(angle(v1, v2), 180.0f * TO_RADS);
    v1 = {-1, -1};
    EXPECT_FLOAT_EQ(angle(v1, v2), 135.0f * TO_RADS);
    v1 = {0, -1};
    EXPECT_FLOAT_EQ(angle(v1, v2), 90.0f * TO_RADS);
    v1 = {1, -1};
    EXPECT_FLOAT_EQ(angle(v1, v2), 45.0f * TO_RADS);
}

TEST(Vec2, Distance) {
    vec2 v1 {45.777,PI};
    vec2 v2 {-3.33,108.22};
    vec2 v3 {21.1,0.005f};
    EXPECT_FLOAT_EQ((v2 - v1).length(), 115.9869);
    EXPECT_FLOAT_EQ((v3 - v2).length(), 110.9383);
    EXPECT_FLOAT_EQ((v3 - v1).length(), 24.87554);
}

TEST(Vec2, Reflect) {
    vec2 v2{1,1};
    vec2 norm{1,1};
    EXPECT_EQ(reflect(v2, norm), vec2(-3,-3));
    v2 = {-1,0};
    EXPECT_EQ(reflect(v2, norm), vec2(1,2));
    v2 = {6,2};
    norm = {14,-13};
    EXPECT_EQ(reflect(v2, norm), vec2(-1618.00,1510.00));
}

TEST(Vec2, Normalize) {
    vec2 v2{};
    v2.normalize();
    EXPECT_EQ(v2, vec2());
    v2 = {1,1};
    v2.normalize();
    EXPECT_EQ(v2, vec2(0.5*sqrt(2)));
    EXPECT_FLOAT_EQ(v2.length(), 1.0f);
    v2 = {-343.0f, 0.556f};
    v2.normalize();
    EXPECT_FLOAT_EQ(v2.length(), 1.0f);
    EXPECT_FLOAT_EQ(v2.length_sq(), 1.0f);
    EXPECT_EQ(v2, (vec2{-0.999999f, 0.001621f}));
}

TEST(Vec2, ToPolar) {
    vec2 v2{1, 1};
    vec2 v3 {to_polar(v2)};
    EXPECT_EQ(v3, vec2(sqrt(2), 45.0f*TO_RADS));
    EXPECT_EQ(to_cartesian(v3), v2);
}


TEST(Vec2, Operators) {
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
}

TEST(Vec2, MinMax) {
    vec2 v1{3,-6};
    vec2 v2{4, 4};
    vec2 v3{-8, 9};
    vec2 v4{-5, -3};
    EXPECT_EQ(min(v1, v1), vec2(3, -6));
    EXPECT_EQ(max(v1, v1), vec2(3, -6));

    EXPECT_EQ(min(v1, v2), vec2(3, -6));
    EXPECT_EQ(max(v1, v2), vec2(4, 4));
    EXPECT_EQ(min(v2, v1), vec2(3, -6));
    EXPECT_EQ(max(v2, v1), vec2(4, 4));

    EXPECT_EQ(min(v1, v3), vec2(-8, -6));
    EXPECT_EQ(max(v1, v3), vec2(3, 9));
    EXPECT_EQ(min(v3, v1), vec2(-8, -6));
    EXPECT_EQ(max(v3, v1), vec2(3, 9));

    EXPECT_EQ(min(v1, v4), vec2(-5, -6));
    EXPECT_EQ(max(v1, v4), vec2(3, -3));
    EXPECT_EQ(min(v4, v1), vec2(-5, -6));
    EXPECT_EQ(max(v4, v1), vec2(3, -3));

    EXPECT_EQ(min(v2, v3), vec2(-8, 4));
    EXPECT_EQ(max(v2, v3), vec2(4, 9));
    EXPECT_EQ(min(v3, v2), vec2(-8, 4));
    EXPECT_EQ(max(v3, v2), vec2(4, 9));

    EXPECT_EQ(min(v2, v4), vec2(-5, -3));
    EXPECT_EQ(max(v2, v4), vec2(4, 4));
    EXPECT_EQ(min(v4, v2), vec2(-5, -3));
    EXPECT_EQ(max(v4, v2), vec2(4, 4));

    EXPECT_EQ(min(v3, v4), vec2(-8, -3));
    EXPECT_EQ(max(v3, v4), vec2(-5, 9));
    EXPECT_EQ(min(v4, v3), vec2(-8, -3));
    EXPECT_EQ(max(v4, v3), vec2(-5, 9));

}

TEST(Vec2, Abs) {
    vec2 rnd = {7.5, 5.0};
    EXPECT_EQ(abs(rnd), vec2(7.5,5.0));
    rnd = {-7.5, 5.0};
    EXPECT_EQ(abs(rnd), vec2(7.5,5.0));
    rnd = {-7.5, -5.0};
    EXPECT_EQ(abs(rnd), vec2(7.5,5.0));
    rnd = {7.5, -5.0};
    EXPECT_EQ(abs(rnd), vec2(7.5,5.0));
}

TEST(Vec2, Ceil) {
    vec2 rnd = {0.445f, 0.557f};
    EXPECT_EQ(ceil(rnd), vec2(1,1));
    rnd = {-34.445123f, -0.5888899f};
    EXPECT_EQ(ceil(rnd), vec2(-34,0));
    rnd = {34.445123f, 9.5888899f};
    EXPECT_EQ(ceil(rnd), vec2(35,10));
}

TEST(Vec2, Floor) {
    vec2 rnd = {0.445f, 0.557f};
    EXPECT_EQ(floor(rnd), vec2(0,0));
    rnd = {-34.445123f, -0.5888899f};
    EXPECT_EQ(floor(rnd), vec2(-35,-1));
    rnd = {34.445123f, 9.5888899f};
    EXPECT_EQ(floor(rnd), vec2(34,9));
}

TEST(Vec2, Round) {
    vec2 rnd = {0.445f, 0.557f};
    EXPECT_EQ(round(rnd), vec2(0,1));
    EXPECT_EQ(round(rnd, 2), vec2(0.44, 0.56));
    rnd = {-34.445123f, -0.5888899f};
    EXPECT_EQ(round(rnd), vec2(-34,-1));
    EXPECT_EQ(round(rnd, 4), vec2(-34.4451, -0.5889));
}
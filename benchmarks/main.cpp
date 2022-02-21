#include <benchmark/benchmark.h>
#include "math/matrix4.h"

//#define MISC_BENCH
//#define MAT4_BENCH
//#define MAT3_BENCH
#define MAT2_BENCH
//#define VEC4_BENCH
//#define QUAT_BENCH

using namespace noble_steed;


#ifdef MISC_BENCH
static void Square_Root_Float(benchmark::State &state)
{
    float res = 0.0;
    float param = 25.8;
    for (auto _ : state)
         res = math::sqrt(param);
}
BENCHMARK(Square_Root_Float);

static void Recip_Square_Root_Float(benchmark::State &state)
{
    float res = 0.0;
    float param = 25.8;
    for (auto _ : state)
         res = math::rsqrt(param);
}
BENCHMARK(Recip_Square_Root_Float);
#endif


#ifdef MAT4_BENCH
// Define another benchmark
static void Mat4_Mult(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 m2{4*v, 3*v, 2*v, v};
    mat4 res;
    for (auto _ : state)
    {
         res = m * m2;
    }
}
BENCHMARK(Mat4_Mult);

static void Mat4_Div(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 m2{4*v, 3*v, 2*v, v};
    mat4 res;
    for (auto _ : state)
    {
         res = m / m2;
    }
}
BENCHMARK(Mat4_Div);


static void Mat4_Comp_Mult(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 m2{4*v, 3*v, 2*v, v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::compwise_mult(m, m2);
    }
}
BENCHMARK(Mat4_Comp_Mult);

static void Mat4_Comp_Div(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 m2{4*v, 3*v, 2*v, v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::compwise_div(m, m2);
    }
}
BENCHMARK(Mat4_Comp_Div);

static void Mat4_Comp_Row_Mult(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::compwise_mult_rows(m, v);
    }
}
BENCHMARK(Mat4_Comp_Row_Mult);

static void Mat4_Comp_Row_Div(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::compwise_div_rows(m, v);
    }
}
BENCHMARK(Mat4_Comp_Row_Div);


static void Mat4_Comp_Col_Mult(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::compwise_mult_columns(m, v);
    }
}
BENCHMARK(Mat4_Comp_Col_Mult);

static void Mat4_Comp_Col_Div(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::compwise_div_columns(m, v);
    }
}
BENCHMARK(Mat4_Comp_Col_Div);

static void Mat4_Transpose(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::transpose(m);
    }
}
BENCHMARK(Mat4_Transpose);

static void Mat4_Determinant(benchmark::State &state)
{
    mat4 m = math::perspective(90.0f, 9/16.0f, 1.0f, 100.0f);
    float res;
    for (auto _ : state)
    {
         res = math::determinant(m);
    }

}
BENCHMARK(Mat4_Determinant);

static void Mat4_Inverse(benchmark::State &state)
{
    mat4 m = math::perspective(90.0f, 9/16.0f, 1.0f, 100.0f);
    mat4 res;
    for (auto _ : state)
    {
         res = math::inverse(m);
    }
}
BENCHMARK(Mat4_Inverse);

static void Mat4_Vec4_Mult(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    vec4 res;
    for (auto _ : state)
    {
         res = m * v;
    }
}
BENCHMARK(Mat4_Vec4_Mult);

static void Vec4_Mat4_Mult(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    vec4 res;
    for (auto _ : state)
    {
         res = v * m;
    }
}
BENCHMARK(Vec4_Mat4_Mult);

static void Mat4_Vec4_Div(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    vec4 res;
    for (auto _ : state)
    {
         res = m / v;
    }
}
BENCHMARK(Mat4_Vec4_Div);

static void Vec4_Mat4_Div(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    vec4 res;
    for (auto _ : state)
    {
         res = v / m;
    }
}
BENCHMARK(Vec4_Mat4_Div);

// Define another benchmark
static void Mat4_Add(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 m2{4*v, 3*v, 2*v, v};
    mat4 res;
    for (auto _ : state)
    {
         res = m + m2;
    }
}
BENCHMARK(Mat4_Add);

// Define another benchmark
static void Mat4_Sub(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 m2{4*v, 3*v, 2*v, v};
    mat4 res;
    for (auto _ : state)
    {
         res = m - m2;
    }
}
BENCHMARK(Mat4_Sub);

// Define another benchmark
static void Mat4_Scalar_Mult(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = m * 2.5f;
    }
}
BENCHMARK(Mat4_Scalar_Mult);

// Define another benchmark
static void Mat4_Scalar_Div(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = m / 2.5f;
    }
}
BENCHMARK(Mat4_Scalar_Div);
#endif


#ifdef MAT3_BENCH
// Define another benchmark
static void Mat3_Mult(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 m2{4*v, 3*v, 2*v};
    mat3 res;
    for (auto _ : state)
    {
         res = m * m2;
    }
}
BENCHMARK(Mat3_Mult);

static void Mat3_Div(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 m2{4*v, 3*v, 2*v};
    mat3 res;
    for (auto _ : state)
    {
         res = m / m2;
    }
}
BENCHMARK(Mat3_Div);


static void Mat3_Comp_Mult(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 m2{4*v, 3*v, 2*v};
    mat3 res;
    for (auto _ : state)
    {
         res = math::compwise_mult(m, m2);
    }
}
BENCHMARK(Mat3_Comp_Mult);

static void Mat3_Comp_Div(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 m2{4*v, 3*v, 2*v};
    mat3 res;
    for (auto _ : state)
    {
         res = math::compwise_div(m, m2);
    }
}
BENCHMARK(Mat3_Comp_Div);

static void Mat3_Comp_Row_Mult(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 res;
    for (auto _ : state)
    {
         res = math::compwise_mult_rows(m, v);
    }
}
BENCHMARK(Mat3_Comp_Row_Mult);

static void Mat3_Comp_Row_Div(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 res;
    for (auto _ : state)
    {
         res = math::compwise_div_rows(m, v);
    }
}
BENCHMARK(Mat3_Comp_Row_Div);


static void Mat3_Comp_Col_Mult(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 res;
    for (auto _ : state)
    {
         res = math::compwise_mult_columns(m, v);
    }
}
BENCHMARK(Mat3_Comp_Col_Mult);

static void Mat3_Comp_Col_Div(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 res;
    for (auto _ : state)
    {
         res = math::compwise_div_columns(m, v);
    }
}
BENCHMARK(Mat3_Comp_Col_Div);

static void Mat3_Transpose(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    mat3 res;
    for (auto _ : state)
    {
         res = math::transpose(m);
    }
}
BENCHMARK(Mat3_Transpose);

static void Mat3_Determinant(benchmark::State &state)
{
    mat3 m = math::basis(math::perspective(90.0f, 9/16.0f, 1.0f, 100.0f));
    float res;
    for (auto _ : state)
    {
         res = math::determinant(m);
    }

}
BENCHMARK(Mat3_Determinant);

static void Mat3_Inverse(benchmark::State &state)
{
    mat3 m = math::basis(math::perspective(90.0f, 9/16.0f, 1.0f, 100.0f));
    mat3 res;
    for (auto _ : state)
    {
         res = math::inverse(m);
    }
}
BENCHMARK(Mat3_Inverse);

static void Mat3_Vec3_Mult(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    vec3 res;
    for (auto _ : state)
    {
         res = m * v;
    }
}
BENCHMARK(Mat3_Vec3_Mult);

static void Vec3_Mat3_Mult(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    vec3 res;
    for (auto _ : state)
    {
         res = v * m;
    }
}
BENCHMARK(Vec3_Mat3_Mult);

static void Mat3_Vec3_Div(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    vec3 res;
    for (auto _ : state)
    {
         res = m / v;
    }
}
BENCHMARK(Mat3_Vec3_Div);

static void Vec3_Mat3_Div(benchmark::State &state)
{
    vec3 v{1, 2, 3};
    mat3 m{v, 2*v, 3*v};
    vec3 res;
    for (auto _ : state)
    {
         res = v / m;
    }
}
BENCHMARK(Vec3_Mat3_Div);

// Define another benchmark
static void Mat3_Add(benchmark::State &state)
{
    vec3 v{1, 2.2, 3.5};
    mat3 m{v, 2*v, 3*v};
    mat3 m2{4*v, 3*v, 2*v};
    mat3 res;
    for (auto _ : state)
    {
         res = m + m2;
    }
}
BENCHMARK(Mat3_Add);

// Define another benchmark
static void Mat3_Sub(benchmark::State &state)
{
    vec3 v{1, 2.2, 3.5};
    mat3 m{v, 2*v, 3*v};
    mat3 m2{4*v, 3*v, 2*v};
    mat3 res;
    for (auto _ : state)
    {
         res = m - m2;
    }
}
BENCHMARK(Mat3_Sub);

// Define another benchmark
static void Mat3_Scalar_Mult(benchmark::State &state)
{
    vec3 v{1, 2.2, 3.5};
    mat3 m{v, 2*v, 3*v};
    mat3 res;
    for (auto _ : state)
    {
         res = m * 2.5f;
    }
}
BENCHMARK(Mat3_Scalar_Mult);

// Define another benchmark
static void Mat3_Scalar_Div(benchmark::State &state)
{
    vec3 v{1, 2.2, 3.5};
    mat3 m{v, 2*v, 3*v};
    mat3 res;
    for (auto _ : state)
    {
         res = m / 2.5f;
    }
}
BENCHMARK(Mat3_Scalar_Div);
#endif


#ifdef MAT2_BENCH
// Define another benchmark
static void Mat2_Mult(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 m2{4*v, 3*v};
    mat2 res;
    for (auto _ : state)
    {
         res = m * m2;
    }
}
BENCHMARK(Mat2_Mult);

static void Mat2_Div(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 m2{4*v, 3*v};
    mat2 res;
    for (auto _ : state)
    {
         res = m / m2;
    }
}
BENCHMARK(Mat2_Div);


static void Mat2_Comp_Mult(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 m2{4*v, 3*v};
    mat2 res;
    for (auto _ : state)
    {
         res = math::compwise_mult(m, m2);
    }
}
BENCHMARK(Mat2_Comp_Mult);

static void Mat2_Comp_Div(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 m2{4*v, 3*v};
    mat2 res;
    for (auto _ : state)
    {
         res = math::compwise_div(m, m2);
    }
}
BENCHMARK(Mat2_Comp_Div);

static void Mat2_Comp_Row_Mult(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 res;
    for (auto _ : state)
    {
         res = math::compwise_mult_rows(m, v);
    }
}
BENCHMARK(Mat2_Comp_Row_Mult);

static void Mat2_Comp_Row_Div(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 res;
    for (auto _ : state)
    {
         res = math::compwise_div_rows(m, v);
    }
}
BENCHMARK(Mat2_Comp_Row_Div);


static void Mat2_Comp_Col_Mult(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 res;
    for (auto _ : state)
    {
         res = math::compwise_mult_columns(m, v);
    }
}
BENCHMARK(Mat2_Comp_Col_Mult);

static void Mat2_Comp_Col_Div(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 res;
    for (auto _ : state)
    {
         res = math::compwise_div_columns(m, v);
    }
}
BENCHMARK(Mat2_Comp_Col_Div);

static void Mat2_Transpose(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    mat2 res;
    for (auto _ : state)
    {
         res = math::transpose(m);
    }
}
BENCHMARK(Mat2_Transpose);

static void Mat2_Determinant(benchmark::State &state)
{
    mat2 m = math::basis(math::basis(math::perspective(90.0f, 9/16.0f, 1.0f, 100.0f)));
    float res;
    for (auto _ : state)
    {
         res = math::determinant(m);
    }

}
BENCHMARK(Mat2_Determinant);

static void Mat2_Inverse(benchmark::State &state)
{
    mat2 m = math::basis(math::basis(math::perspective(90.0f, 9/16.0f, 1.0f, 100.0f)));
    mat2 res;
    for (auto _ : state)
    {
         res = math::inverse(m);
    }
}
BENCHMARK(Mat2_Inverse);

static void Mat2_Vec2_Mult(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    vec2 res;
    for (auto _ : state)
    {
         res = m * v;
    }
}
BENCHMARK(Mat2_Vec2_Mult);

static void Vec2_Mat2_Mult(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    vec2 res;
    for (auto _ : state)
    {
         res = v * m;
    }
}
BENCHMARK(Vec2_Mat2_Mult);

static void Mat2_Vec2_Div(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    vec2 res;
    for (auto _ : state)
    {
         res = m / v;
    }
}
BENCHMARK(Mat2_Vec2_Div);

static void Vec2_Mat2_Div(benchmark::State &state)
{
    vec2 v{1, 2};
    mat2 m{v, 2*v};
    vec2 res;
    for (auto _ : state)
    {
         res = v / m;
    }
}
BENCHMARK(Vec2_Mat2_Div);

// Define another benchmark
static void Mat2_Add(benchmark::State &state)
{
    vec2 v{1, 2.2};
    mat2 m{v, 2*v};
    mat2 m2{4*v, 3*v};
    mat2 res;
    for (auto _ : state)
    {
         res = m + m2;
    }
}
BENCHMARK(Mat2_Add);

// Define another benchmark
static void Mat2_Sub(benchmark::State &state)
{
    vec2 v{1, 2.2};
    mat2 m{v, 2*v};
    mat2 m2{4*v, 3*v};
    mat2 res;
    for (auto _ : state)
    {
         res = m - m2;
    }
}
BENCHMARK(Mat2_Sub);

// Define another benchmark
static void Mat2_Scalar_Mult(benchmark::State &state)
{
    vec2 v{1, 2.2};
    mat2 m{v, 2*v};
    mat2 res;
    for (auto _ : state)
    {
         res = m * 2.5f;
    }
}
BENCHMARK(Mat2_Scalar_Mult);

// Define another benchmark
static void Mat2_Scalar_Div(benchmark::State &state)
{
    vec2 v{1, 2.2};
    mat2 m{v, 2*v};
    mat2 res;
    for (auto _ : state)
    {
         res = m / 2.5f;
    }
}
BENCHMARK(Mat2_Scalar_Div);
#endif


#ifdef VEC4_BENCH
// Define another benchmark
static void Vec4_Dot_Product(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    vec4 v2{5, 6, 7, 8};
    for (auto _ : state)
    {
        float ret = math::dot(v, v2);
    }
}
BENCHMARK(Vec4_Dot_Product);

static void Vec4_Length(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    vec4 res;
    for (auto _ : state)
         res = math::length(v);
}
BENCHMARK(Vec4_Length);

static void Vec4_Normalize(benchmark::State &state)
{
    vec4 param{3.6, 9, -3.55, 0.8};
    vec4 res;
    for (auto _ : state)
         res = math::normalize(param);
}
BENCHMARK(Vec4_Normalize);

// Define another benchmark
static void Vec4_Scalar_Mult(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    vec4 res;
    for (auto _ : state)
    {
         res = v * 2.5f;
    }
}
BENCHMARK(Vec4_Scalar_Mult);

// Define another benchmark
static void Vec4_Scalar_Div(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    vec4 res;
    for (auto _ : state)
    {
         res = v / 2.5f;
    }
}
BENCHMARK(Vec4_Scalar_Div);

// Define another benchmark
static void Vec4_Comp_Mult(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    vec4 v2{5.5, 8.2, -3.5, 4.333};
    vec4 res;
    for (auto _ : state)
    {
         res = v * v2;
    }
}
BENCHMARK(Vec4_Comp_Mult);

// Define another benchmark
static void Vec4_Comp_Div(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    vec4 v2{5.5, 8.2, -3.5, 4.333};
    vec4 res;
    for (auto _ : state)
    {
         res = v / v2;
    }
}
BENCHMARK(Vec4_Comp_Div);

// Define another benchmark
static void Vec4_Add(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    vec4 v2{5.5, 8.2, -3.5, 4.333};
    vec4 res;
    for (auto _ : state)
    {
         res = v + v2;
    }
}
BENCHMARK(Vec4_Add);

// Define another benchmark
static void Vec4_Sub(benchmark::State &state)
{
    vec4 v{1, 2.2, 3.5, 4};
    vec4 v2{5.5, 8.2, -3.5, 4.333};
    vec4 res;
    for (auto _ : state)
    {
         res = v - v2;
    }
}
BENCHMARK(Vec4_Sub);
#endif

#ifdef QUAT_BENCH
// Define another benchmark
static void Quat_Dot(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    quat v2{5.5, 8.2, -3.5, 4.333};
    float res;
    for (auto _ : state)
    {
         res = math::dot(v, v2);
    }
}
BENCHMARK(Quat_Dot);

// Define another benchmark
static void Quat_Norm(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    quat res;
    for (auto _ : state)
    {
         res = math::normalize(v);
    }
}
BENCHMARK(Quat_Norm);

// Define another benchmark
static void Quat_Length(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    float res;
    for (auto _ : state)
    {
         res = math::length(v);
    }
}
BENCHMARK(Quat_Length);

// Define another benchmark
static void Quat_Mult_Scalar(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    quat res;
    for (auto _ : state)
    {
         res = v * 5.7;
    }
}
BENCHMARK(Quat_Mult_Scalar);

// Define another benchmark
static void Quat_Divide_Scalar(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    quat res;
    for (auto _ : state)
    {
         res = v / 5.7;
    }
}
BENCHMARK(Quat_Divide_Scalar);

// Define another benchmark
static void Quat_Add(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    quat v2{5.3, 1.55, -4.321, -2};
    quat res;
    for (auto _ : state)
    {
         res = v + v2;
    }
}
BENCHMARK(Quat_Add);

// Define another benchmark
static void Quat_Subtract(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    quat v2{5.3, 1.55, -4.321, -2};
    quat res;
    for (auto _ : state)
    {
         res = v - v2;
    }
}
BENCHMARK(Quat_Subtract);

// Define another benchmark
static void Quat_Quat_Mult(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    quat v2{5.3, 1.55, -4.321, -2};
    quat res;
    for (auto _ : state)
    {
         res = v * v2;
    }
}
BENCHMARK(Quat_Quat_Mult);

// Define another benchmark
static void Quat_Vec3_Mult(benchmark::State &state)
{
    quat v{1, 2.2, 3.5, 4};
    vec3 v2{5.3, 1.55, -4.321};
    vec3 res;
    for (auto _ : state)
    {
         res = v * v2;
    }
}
BENCHMARK(Quat_Vec3_Mult);
#endif
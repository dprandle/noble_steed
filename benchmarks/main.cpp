#include <benchmark/benchmark.h>
#include "math/vector4.h"
#include "math/matrix4.h"

using namespace noble_steed;

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

static void Vec4_Length(benchmark::State &state)
{
    dvec4 v{1, 2, 3, 4};
    dvec4 res;
    for (auto _ : state)
         res = math::length(v);
}
BENCHMARK(Vec4_Length);

static void Square_Root_Double(benchmark::State &state)
{
    double res = 0.0;
    double param = 25.8;    
    for (auto _ : state)
         res = math::sqrt(param);
}
BENCHMARK(Square_Root_Double);

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

static void Normalize(benchmark::State &state)
{
    vec4 param{3.6, 9, -3.55, 0.8};
    vec4 res;
    for (auto _ : state)
         res = math::normalize(param);
}
BENCHMARK(Normalize);

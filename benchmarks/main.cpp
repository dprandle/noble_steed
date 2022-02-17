#include <benchmark/benchmark.h>
#include "math/vector4.h"
#include "math/matrix4.h"

using namespace noble_steed;

// Define another benchmark
static void Vec4DotProduct(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    vec4 v2{5, 6, 7, 8};
    for (auto _ : state)
    {
        float ret = math::dot(v, v2);
    }
}
BENCHMARK(Vec4DotProduct);

// Define another benchmark
static void Mat4Mult(benchmark::State &state)
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
BENCHMARK(Mat4Mult);

static void Mat4Transpose(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    mat4 res;
    for (auto _ : state)
    {
         res = math::transpose(m);
    }
}
BENCHMARK(Mat4Transpose);

static void Mat4Vec4Mult(benchmark::State &state)
{
    vec4 v{1, 2, 3, 4};
    mat4 m{v, 2*v, 3*v, 4*v};
    vec4 res;
    for (auto _ : state)
    {
         res = m * v;
    }
}
BENCHMARK(Mat4Vec4Mult);
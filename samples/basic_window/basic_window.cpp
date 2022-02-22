#include "core/application.h"
#include "core/resource_cache.h"
#include "core/context.h"
#include "core/profile_timer.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/mesh.h"

#include "io/input_map.h"
#include "io/input_translator.h"

#include "math/bounding_box.h"
#include "math/quaternion.h"

#include "scene/world.h"
#include "scene/world_chunk.h"
#include "scene/world.h"
#include "scene/entity.h"

#include "container/string.h"
#include <chrono>

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace noble_steed;

int main()
{
    // mat4 m = math::perspective(90.0f, 9/16.0f, 1.0f, 100.0f);
    float data1[4][4] = {{1.7f, 3.2f, 0.4f, 6.2f}, {4.3f, 5.0f, 0.4f,6.2f}, {4.3f, 5.0f, 0.4f, 6.2f}, {4.3f, 5.0f, 0.4f, 6.2f}};
    float data2[16] = {1.7f, 3.2f, 4.3f, 5.0f, 1.7f, 3.2f, 4.3f, 5.0f, 6.5f, 4.3f, 5.0f, 1.7f, 3.2f, 4.3f, 5.0f, 6.5f};
    mat4 m2(data1);
    mat4 m3(data2);
    std::cout << "data1: " << m2 << std::endl;
    std::cout << "data2: " << m3 << std::endl;

    mat3 m({1,2, 3},{4, 5, 6}, {7, 8, 9});
    vec3 v(3.2, 6, 5.3);
    std::cout << "v: " << v << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "m': " << math::transpose(m) << std::endl;
    std::cout << "m*v: " << m*v << std::endl;
    std::cout << "\ndot(v,v): " << math::dot(v, v) << std::endl;
    std::cout << "\nv*m: " << v*m << std::endl;
    std::cout << "\nm/v: " << m/v << std::endl;
    std::cout << "\nv/m: " << v/m << std::endl;
    std::cout << "\nmult rows:" << math::compwise_mult_rows(m, v);
    std::cout << "\nmult columns:" << math::compwise_mult_columns(m, v);

    std::cout << "\ndiv (1) rows:" << math::compwise_div_rows(m, v);
    std::cout << "\ndiv (1) columns:" << math::compwise_div_columns(m, v);

    std::cout << "\ndiv (1) rows:" << math::compwise_div_rows(v, m);
    std::cout << "\ndiv (1) columns:" << math::compwise_div_columns(v, m);

    return 0;
}

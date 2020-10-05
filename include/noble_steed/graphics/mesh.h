#pragma once
#include <bgfx/bgfx.h>

#include "../core/resource.h"
#include "../core/common.h"
#include "../math/vec3.h"
#include "../math/vec2.h"
#include "../math/vec4.h"
#include "../container/vector.h"

namespace noble_steed
{
struct Vertex_Data
{
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 tex_coord;
    u32 color;
    static void initialize();
    static bgfx::VertexLayout layout_;
};

struct Submesh
{
    Vector<Vertex_Data> vertices;
    Vector<u32> indices;
};

class Mesh: public Resource
{
    public:

    Submesh * get_submesh(u32 index);
    void add_submesh(const Submesh & subm);
    void remove_submesh(u32 index);

    private:
    Vector<Submesh> submeshes_;
};

}
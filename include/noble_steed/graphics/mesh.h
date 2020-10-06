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

struct Vertex_Buffer
{
    bgfx::VertexBufferHandle handle;
    bgfx::VertexLayout layout;
    String name;
};

struct Submesh
{
    Vector<Vertex_Buffer> buffers;
    Vector<vec3> positions;
    Vector<vec3> normals;
    Vector<vec3> tangents;
    Vector<vec3> bitangents;
    Vector<vec2> tex_coords;
};

class Mesh: public Resource
{
    public:

    Submesh * get_submesh(u32 index);
    void add_submesh(const Submesh & subm);
    void remove_submesh(u32 index);
    u32 submesh_count();

    Signal<u32,u32> submesh_id_change;

    private:
    Vector<Submesh> submeshes_;
};

}
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
    bgfx::VertexLayout layout;
    bgfx::VertexBufferHandle handle;
};

struct Vertex_Buffer : public Vertex_Buffer
{
    Vector<T> buffer_data;
};

struct Submesh
{
    Hash_Map<u32, Vertex_Buffer*>
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
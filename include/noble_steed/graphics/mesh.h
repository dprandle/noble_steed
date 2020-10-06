#pragma once
#include <bgfx/bgfx.h>

#include "../core/resource.h"
#include "../core/common.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../container/vector.h"


BGFX_HANDLE(Index_Buffer_Handle)


namespace noble_steed
{
struct Vertex_Buffer
{
    Vertex_Buffer();

    /// The buffer handle as provided by bgfx. This will either be a static or dynamic handle based on \ref Vertex_Buffer.dynamic. The layout
    /// is given by \ref Vertex_Buffer.layout.
    u16 handle;

    /// The debug name of this buffer.
    String name;

    /// The bgfx flags used in creating this buffer.
    u16 flags;

    /// The bgfx layout of the vertex buffer.
    bgfx::VertexLayout layout;

    /// Pointer to data to be passed to bgfx on creating vertex buffer. Data is not owned by buffer - if adding custom vertex buffer
    /// make sure to manage data lifetime. This data is passed to bgfx via makeRef - we'll see if the two frame thing causes issues.
    void * data;

    /// Size of \param data in bytes
    u32 data_size;

    /// Is this buffer dynamic - ie vertice information may change frame to frame
    bool dynamic;

    void alloc();
    void update(u32 offset = 0);
    void free();
};

struct Index_Buffer
{
    Index_Buffer();

    /// The buffer handle as provided by bgfx. This will either be a static or dynamic handle based on \ref Vertex_Buffer.flags. The layout
    /// is given by \ref Index_Buffer.dynamic.
    u16 handle;

    /// The debug name of this buffer.
    String name;

    /// The bgfx flags used in creating this buffer.
    u16 flags;

    /// Pointer to data to be passed to bgfx on creating index buffer. Data is not owned by buffer - if adding custom idnex buffer
    /// make sure to manage data lifetime. This data is passed to bgfx via makeRef - we'll see if the two frame thing causes issues.
    Vector<u32> data;

    /// Is this buffer dynamic - ie vertice information may change frame to frame
    bool dynamic;

    void alloc();
    void update(u32 offset);
    void free();
};

struct Bone_Weight
{
    Bone_Weight();

    uvec4 indices;
    vec4 weights;
};

template<class T>
struct Vertex_Buffer_Data
{
    Vertex_Buffer_Data(): buffer(nullptr) {}
    Vertex_Buffer * buffer;
    Vector<T> data;
};

struct Vertex_Data
{
    void alloc_buffers();
    void update_buffers();
    void free_buffers();
    
    Vector<Vertex_Buffer> buffers;

    Vertex_Buffer_Data<vec3> positions;
    Vertex_Buffer_Data<vec3> normals;
    Vertex_Buffer_Data<vec3> tangents;
    Vertex_Buffer_Data<vec3> bitangents;
    Vertex_Buffer_Data<Bone_Weight> bone_weights;
    Vertex_Buffer_Data<u8vec4> colors;
    Vertex_Buffer_Data<u8vec4> colors2;
    Vertex_Buffer_Data<u8vec4> colors3;
    Vertex_Buffer_Data<u8vec4> colors4;
    Vertex_Buffer_Data<vec2> tex_coords_1;
    Vertex_Buffer_Data<vec2> tex_coords_2;
    Vertex_Buffer_Data<vec2> tex_coords_3;
    Vertex_Buffer_Data<vec2> tex_coords_4;
    Vertex_Buffer_Data<vec2> tex_coords_5;
    Vertex_Buffer_Data<vec2> tex_coords_6;
    Vertex_Buffer_Data<vec2> tex_coords_7;
    Vertex_Buffer_Data<vec2> tex_coords_8;
};

class Mesh;

struct Submesh
{
    Submesh();

    Vertex_Data vert_data;
    Vector<Index_Buffer> index_buffers;
    Mesh * owner;
    bool use_shared_vert_data;
};

class Mesh : public Resource
{
  public:
    Submesh * add_submesh(u32 & sub_ind, const Submesh & copy = Submesh());

    Submesh * get_submesh(u32 index);

    bool remove_submesh(u32 index);

    u32 submesh_count();

    void update_buffers();

    void update_buffers(u32 sub_ind);

    Signal<u32, u32> submesh_id_change;

  private:
    Vector<Submesh> submeshes_;
    Vertex_Data shared_vert_data_;
};

} // namespace noble_steed
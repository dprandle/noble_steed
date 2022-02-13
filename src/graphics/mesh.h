#pragma once
#include <bgfx/bgfx.h>

#include "../core/resource.h"
#include "../container/vector.h"
#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../math/vector4.h"

BGFX_HANDLE(Index_Buffer_Handle)

namespace noble_steed
{

enum Index_Buffer_Type
{
    TRIANGLES,
    TRIANGLE_STRIP,
    LINES,
    LINE_STRIP,
    POINTS,
    COUNT
};

struct vertex_buffer
{
    vertex_buffer();

    /// The buffer handle as provided by bgfx. This will either be a static or dynamic handle based on \ref vertex_buffer.dynamic. The layout
    /// is given by \ref vertex_buffer.layout.
    u16 handle;

    /// The debug name of this buffer.
    String name;

    /// The bgfx flags used in creating this buffer.
    u16 flags;

    /// The bgfx layout of the vertex buffer.
    bgfx::VertexLayout layout;

    /// Pointer to data to be passed to bgfx on creating vertex buffer. Data is not owned by buffer - if adding custom vertex buffer
    /// make sure to manage data lifetime. This data is passed to bgfx via makeRef - we'll see if the two frame thing causes issues.
    void *data;

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

    /// The buffer handle as provided by bgfx. This will either be a static or dynamic handle based on \ref vertex_buffer.flags. The layout
    /// is given by \ref Index_Buffer.dynamic.
    u16 handle;

    /// The debug name of this buffer.
    String name;

    /// The bgfx flags used in creating this buffer.
    u16 flags;

    /// Pointer to data to be passed to bgfx on creating index buffer. Data is not owned by buffer - if adding custom idnex buffer
    /// make sure to manage data lifetime. This data is passed to bgfx via makeRef - we'll see if the two frame thing causes issues.
    vector<u32> data;

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
    Vertex_Buffer_Data() : buffer(nullptr)
    {}
    vertex_buffer *buffer;
    vector<T> data;
};

struct Vertex_Data
{
    /// Create buffers for all data that has at least one vertice
    void create_buffers(const String &name_prefix);

    /// Allocate buffers for all invalid handles - if dynamic and data is null, will just allocate based on size. If not dynamic and data is null,
    /// then don't do anything and log warning.
    void alloc_buffers();

    /// Update all buffers that are set to dynamic and allocated
    void update_buffers(u32 offset);

    /// Free all buffers that have previously been allocated
    void free_buffers();

    /// Remove all unused (not allocated) buffers
    void destroy_buffers();

    vector<vertex_buffer> buffers;

    Vertex_Buffer_Data<vec3> positions;
    Vertex_Buffer_Data<vec3> normals;
    Vertex_Buffer_Data<vec3> tangents;
    Vertex_Buffer_Data<vec3> bitangents;
    Vertex_Buffer_Data<Bone_Weight> bone_weights;
    Vertex_Buffer_Data<u8vec4> colors0;
    Vertex_Buffer_Data<vec2> tex_coords_0;
};

struct Index_Data
{
    /// Create buffers for all data that has at least one vertice
    void create_buffers(const String &name_prefix);

    /// Allocate buffers for all invalid handles - if dynamic and data is null, will just allocate based on size. If not dynamic and data is null,
    /// then don't do anything and log warning.
    void alloc_buffers();

    /// Update all buffers that are set to dynamic and allocated
    void update_buffers(u32 offset);

    /// Free all buffers that have previously been allocated
    void free_buffers();

    /// Remove all unused (not allocated) buffers
    void destroy_buffers();

    vector<Index_Buffer> buffers;
};

class Mesh;

struct Submesh
{
    void create_buffers();

    void alloc_buffers();

    void update_buffers(u32 offset);

    void free_buffers();

    void destroy_buffers();

    Vertex_Data vert_data;
    Index_Data index_buffers;
    Mesh *owner;
    String name;
    bool use_shared_vert_data;
};

class Mesh : public Resource
{
  public:
    Submesh *add_submesh(u32 &sub_ind, const Submesh &copy = Submesh());

    Submesh *get_submesh(u32 index);

    bool remove_submesh(u32 index);

    u32 submesh_count();

    void update_buffers();

    void update_buffers(u32 sub_ind);

    Signal<u32, u32> submesh_id_change;

  private:
    vector<Submesh> submeshes_;
    Vertex_Data shared_vert_data_;
};

} // namespace noble_steed
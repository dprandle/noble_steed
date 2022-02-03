#include "mesh.h"
#include "../io/logger.h"
#include "../core/variant.h"

namespace noble_steed
{

vertex_buffer::vertex_buffer() : handle(bgfx::kInvalidHandle), name(), flags(0), layout(), data(nullptr), data_size(0), dynamic(false)
{}

void vertex_buffer::alloc()
{
    if (bgfx_valid_handle(handle))
    {
        wlog("Cannot allocate already allocated buffer for {} - use free first", name);
        return;
    }
    if (dynamic)
    {
        handle = bgfx::createDynamicVertexBuffer(bgfx::makeRef(data, data_size), layout, flags).idx;
        ilog("Created dynamic vertex buffer named {} of size {} bytes with id: {}", name, data_size, handle);
    }
    else
    {
        handle = bgfx::createVertexBuffer(bgfx::makeRef(data, data_size), layout, flags).idx;
        ilog("Created static vertex buffer named {} of size {} bytes with id: {}", name, data_size, handle);
    }
}

void vertex_buffer::update(u32 offset)
{
    if (dynamic)
    {
        if (bgfx_valid_handle(handle))
        {
            bgfx::DynamicVertexBufferHandle hnd;
            hnd.idx = handle;
            bgfx::update(hnd, offset, bgfx::makeRef(data, data_size));
        }
        else
        {
            wlog("Cannot update vertex buffer {} as its handle is invalid - make sure alloc was called", name);
        }
    }
    else
    {
        wlog("Cannot update static vertex buffer {}. If dynamic buffer is needed, set dynamic to true and free/alloc buffer again");
    }
}

void vertex_buffer::free()
{
    if (dynamic)
    {
        bgfx::DynamicVertexBufferHandle hnd;
        hnd.idx = handle;
        bgfx::destroy(hnd);
    }
    else
    {
        bgfx::VertexBufferHandle hnd;
        hnd.idx = handle;
        bgfx::destroy(hnd);
    }
    handle = -1;
}

Index_Buffer::Index_Buffer() : handle(bgfx::kInvalidHandle), name(), flags(), data(), dynamic(false)
{}

void Index_Buffer::alloc()
{
    if (bgfx_valid_handle(handle))
    {
        wlog("Cannot allocate already allocated buffer for {} - use free first", name);
        return;
    }
    if (dynamic)
    {
        handle = bgfx::createDynamicIndexBuffer(bgfx::makeRef(data.data(), data.size()), flags | BGFX_BUFFER_INDEX32).idx;
        ilog("Created dynamic index buffer named {} of size {} bytes with id: {}", name, data.size(), handle);
    }
    else
    {
        handle = bgfx::createIndexBuffer(bgfx::makeRef(data.data(), data.size()), flags).idx;
        ilog("Created static index buffer named {} of size {} bytes with id: {}", name, data.size(), handle);
    }
}

void Index_Buffer::update(u32 offset)
{
    if (dynamic)
    {
        if (bgfx_valid_handle(handle))
        {
            bgfx::DynamicIndexBufferHandle hnd;
            hnd.idx = handle;
            bgfx::update(hnd, offset, bgfx::makeRef(data.data(), data.size()));
        }
        else
        {
            wlog("Cannot update index buffer {} as its handle is invalid - make sure alloc was called", name);
        }
    }
    else
    {
        wlog("Cannot update static index buffer {}. If dynamic buffer is needed, set dynamic to true and free/alloc buffer again");
    }
}

void Index_Buffer::free()
{
    if (dynamic)
    {
        bgfx::DynamicIndexBufferHandle hnd;
        hnd.idx = handle;
        bgfx::destroy(hnd);
    }
    else
    {
        bgfx::IndexBufferHandle hnd;
        hnd.idx = handle;
        bgfx::destroy(hnd);
    }
    handle = -1;
}

template<class T>
void create_if_needed(Vertex_Buffer_Data<T> &buf,
                      vector<vertex_buffer> &buffers,
                      const String &name,
                      bgfx::Attrib::Enum attrib,
                      u8 elements,
                      bgfx::AttribType::Enum att_type,
                      bool normalize = false)
{
    if (!buf.data.empty())
    {
        if (!buf.buffer)
        {
            buffers.resize(buffers.size() + 1);
            buf.buffer = &buffers.back();
        }
        if (!bgfx_valid_handle(buf.buffer->handle))
        {
            buf.buffer->data = buf.data.data();
            buf.buffer->data_size = buf.data.size();
            buf.buffer->name = name;
            buf.buffer->layout.begin().add(attrib, elements, att_type, normalize).end();
        }
    }
}

void Vertex_Data::create_buffers(const String &name_prefix)
{
    create_if_needed(positions, buffers, name_prefix + ".verts.positions", bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
    create_if_needed(normals, buffers, name_prefix + ".verts.normals", bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);
    create_if_needed(tangents, buffers, name_prefix + ".verts.tangents", bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float);
    create_if_needed(bitangents, buffers, name_prefix + ".verts.bitangents", bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float);

    if (!bone_weights.data.empty())
    {
        if (!bone_weights.buffer)
        {
            buffers.resize(buffers.size() + 1);
            bone_weights.buffer = &buffers.back();
        }
        if (!bgfx_valid_handle(bone_weights.buffer->handle))
        {
            bone_weights.buffer->data = bone_weights.data.data();
            bone_weights.buffer->data_size = bone_weights.data.size();
            bone_weights.buffer->name = name_prefix + ".verts.bone_weights";
            bone_weights.buffer->layout.begin()
                .add(bgfx::Attrib::Indices, 4, bgfx::AttribType::Int16, false)
                .add(bgfx::Attrib::Weight, 4, bgfx::AttribType::Float, false)
                .end();
        }
    }

    //create_if_needed(bone_weights, "bone_weights", bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
    create_if_needed(colors0, buffers, name_prefix + ".verts.colors0", bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);
    create_if_needed(tex_coords_0, buffers, name_prefix + ".verts.tex_coords_0", bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
}

void Vertex_Data::alloc_buffers()
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (!bgfx_valid_handle(buffers[i].handle))
            buffers[i].alloc();
    }
}

void Vertex_Data::update_buffers(u32 offset)
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (bgfx_valid_handle(buffers[i].handle) && buffers[i].dynamic)
            buffers[i].update(offset);
    }
}

void Vertex_Data::free_buffers()
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (bgfx_valid_handle(buffers[i].handle))
            buffers[i].free();
    }
}

void Vertex_Data::destroy_buffers()
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (!bgfx_valid_handle(buffers[i].handle))
        {
            u32 ind = buffers.size() - 1;
            if (i != ind)
            {
                buffers[i] = buffers[ind];
                --i;
            }
            buffers.pop_back();
        }
    }
}

void Index_Data::create_buffers(const String &name_prefix)
{
    buffers.resize(COUNT);
    buffers[TRIANGLES].name = name_prefix + ".indices.triangles";
    buffers[TRIANGLE_STRIP].name = name_prefix + ".indices.triangle_strip";
    buffers[LINES].name = name_prefix + ".indices.lines";
    buffers[LINE_STRIP].name = name_prefix + ".indices.line_strip";
    buffers[POINTS].name = name_prefix + ".indices.points";
}

void Index_Data::alloc_buffers()
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (!bgfx_valid_handle(buffers[i].handle))
            buffers[i].alloc();
    }
}

void Index_Data::update_buffers(u32 offset)
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (bgfx_valid_handle(buffers[i].handle) && buffers[i].dynamic)
            buffers[i].update(offset);
    }
}

void Index_Data::free_buffers()
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (bgfx_valid_handle(buffers[i].handle))
            buffers[i].free();
    }
}

void Index_Data::destroy_buffers()
{
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (!bgfx_valid_handle(buffers[i].handle))
        {
            u32 ind = buffers.size() - 1;
            if (i != ind)
            {
                buffers[i] = buffers[ind];
                --i;
            }
            buffers.pop_back();
        }
    }
}

void Submesh::create_buffers()
{
    vert_data.create_buffers(name);
    index_buffers.create_buffers(name);
}

void Submesh::alloc_buffers()
{
    vert_data.alloc_buffers();
    index_buffers.alloc_buffers();
}

void Submesh::update_buffers(u32 offset)
{
    vert_data.update_buffers(offset);
    index_buffers.update_buffers(offset);
}

void Submesh::free_buffers()
{
    vert_data.free_buffers();
    index_buffers.free_buffers();
}

void Submesh::destroy_buffers()
{
    vert_data.destroy_buffers();
    index_buffers.destroy_buffers();
}

Submesh *Mesh::get_submesh(u32 index)
{
    if (index > submeshes_.size())
        return nullptr;
    return &submeshes_[index];
}

Submesh *Mesh::add_submesh(u32 &sub_ind, const Submesh &copy)
{
    sub_ind = submeshes_.size();
    submeshes_.push_back(copy);
    submeshes_[sub_ind].owner = this;
    submeshes_[sub_ind].name = get_relative_path();
    return &submeshes_[sub_ind];
}

bool Mesh::remove_submesh(u32 index)
{
    if (index >= submeshes_.size())
        return false;
    u32 old_ind = submeshes_.size() - 1;
    if (index == old_ind)
    {
        submeshes_.pop_back();
        return true;
    }
    submeshes_[index] = submeshes_.back();
    submeshes_.pop_back();
    return true;
}

u32 Mesh::submesh_count()
{
    return submeshes_.size();
}

} // namespace noble_steed

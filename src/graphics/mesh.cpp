#include "noble_steed/graphics/mesh.h"
#include "noble_steed/io/logger.h"

namespace noble_steed
{
Vertex_Buffer::Vertex_Buffer() : handle(bgfx::kInvalidHandle), name(), flags(0), layout(), data(nullptr), data_size(0), dynamic(false)
{}

void Vertex_Buffer::alloc()
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

void Vertex_Buffer::update(u32 offset)
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

void Vertex_Buffer::free()
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

void Vertex_Data::alloc_buffers()
{

}

void Vertex_Data::update_buffers()
{}

void Vertex_Data::free_buffers()
{}

Submesh * Mesh::get_submesh(u32 index)
{
    if (index > submeshes_.size())
        return nullptr;
    return &submeshes_[index];
}

Submesh * Mesh::add_submesh(u32 & sub_ind, const Submesh & copy)
{
    sub_ind = submeshes_.size();
    submeshes_.push_back(copy);
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

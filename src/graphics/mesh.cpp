#include "noble_steed/graphics/mesh.h"

namespace noble_steed
{
bgfx::VertexLayout Vertex_Data::layout_;

void Vertex_Data::initialize()
{
    layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);
}

Submesh * Mesh::get_submesh(u32 index)
{
    if (index > submeshes_.size())
        return nullptr;
    return 
}

void Mesh::add_submesh(const Submesh & subm)
{
    submeshes_.push_back(subm);
}

void Mesh::remove_submesh(u32 index)
{
    if (index >= submeshes_.size())
        return;
    u32 old_ind = submeshes_.size()-1;
    if (index == old_ind)
    {
        submeshes_.pop_back();
        return;
    }
    submeshes_[index]
}

u32 Mesh::submesh_count()
{
    return submeshes_.size();
}

} // namespace noble_steed

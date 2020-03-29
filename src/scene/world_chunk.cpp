#include <noble_steed/scene/world_chunk.h>
#include <noble_steed/core/context.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/entity.h>

namespace noble_steed
{
World_Chunk::World_Chunk()
{}

World_Chunk::~World_Chunk()
{}

void World_Chunk::initialize(const Variant_Map & init_params)
{
    Resource::initialize(init_params);
}

void World_Chunk::terminate()
{
    clear();
    Resource::terminate();
}

void World_Chunk::clear()
{
    while (ents_.begin() != ents_.end())
        remove(ents_.begin()->first,true);
    ents_ptrs_.clear();
}

bool World_Chunk::add(Entity * to_add, const Variant_Map & init_params)
{
    auto added = ents_.emplace(to_add->get_id(), to_add);
    if (added.second)
    {
        to_add->add<Transform>(init_params);
        return true;
    }
    return false;
}

Entity * World_Chunk::add(const Variant_Map & init_params)
{
    Entity * ent = ns_ctxt.get_world()->create(nullptr, init_params);
    if (add(ent, init_params))
        return ent;
    bool dest = ns_ctxt.get_world()->destroy(ent);
    assert(dest);
    return nullptr;
}

Entity * World_Chunk::get(u32 id)
{
    auto fiter = ents_.find(id);
    if (fiter != ents_.end())
        return fiter->second;
    return nullptr;
}

bool World_Chunk::remove(u32 id, bool remove_from_world)
{
    return remove(get(id));
}

bool World_Chunk::remove(Entity * ent, bool remove_from_world)
{
    if (ent == nullptr)
        return false;

    sizet cnt = ents_.erase(ent->get_id());
    if (cnt != 1)
        return false;

    ent->remove<Transform>();

    bool ret = true;

    if (remove_from_world)
        ret = ns_ctxt.get_world()->destroy(ent);
    return ret;
}

void World_Chunk::pack_begin(JSON_Archive::Direction io_dir)
{
    if (io_dir == JSON_Archive::DIR_OUT)
    {
        auto iter = ents_.begin();
        while (iter != ents_.end())
        {
            ents_ptrs_.push_back(iter->second);
            ++iter;
        }
    }
    else
    {
        clear();
    }
}

void World_Chunk::pack_end(JSON_Archive::Direction io_dir)
{
    if (io_dir == JSON_Archive::DIR_OUT)
    {
        ents_ptrs_.clear();
    }
    else
    {
        auto iter = ents_ptrs_.begin();
        while (iter != ents_ptrs_.end())
        {
            ents_[(*iter)->get_id()] = *iter;
            ++iter;
        }
    }
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<World_Chunk>("noble_steed::World_Chunk")
        .constructor<>()
        .property("entities", &World_Chunk::ents_ptrs_, registration::public_access);
}
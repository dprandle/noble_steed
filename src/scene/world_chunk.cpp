#include <noble_steed/scene/world_chunk.h>
#include <noble_steed/core/context.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/entity.h>

namespace noble_steed
{
World_Chunk::World_Chunk() : Resource()
{
    sig_connect(ns_ctxt.get_world()->entity_id_change, this, &World_Chunk::on_ent_id_change);
    sig_connect(ns_ctxt.get_world()->entity_destroyed, this, &World_Chunk::on_ent_destroyed);
}

World_Chunk::World_Chunk(const World_Chunk & copy) : Resource(copy), ents_ptrs_(), ents_()
{
    sig_connect(ns_ctxt.get_world()->entity_id_change, this, &World_Chunk::on_ent_id_change);
    sig_connect(ns_ctxt.get_world()->entity_destroyed, this, &World_Chunk::on_ent_destroyed);
    
    auto iter = copy.ents_.begin();
    while (iter != copy.ents_.end())
    {
        add(*(iter->second));
        ++iter;
    }
}

World_Chunk & World_Chunk::operator=(World_Chunk rhs)
{
    swap(rhs);
    return *this;
}

void World_Chunk::swap(World_Chunk & rhs)
{
    Resource::swap(rhs);
    std::swap(ents_ptrs_, rhs.ents_ptrs_);
    std::swap(ents_, rhs.ents_);
}

World_Chunk::~World_Chunk()
{
    clear(true);
}

void World_Chunk::initialize(const Variant_Map & init_params)
{
    Resource::initialize(init_params);
}

void World_Chunk::terminate()
{
    Resource::terminate();
    clear(true);
}

void World_Chunk::clear(bool remove_entities_from_world)
{
    while (ents_.begin() != ents_.end())
        remove(ents_.begin()->first, remove_entities_from_world);
    ents_ptrs_.clear();
}

bool World_Chunk::add(Entity * to_add, const Variant_Map & init_params)
{
    auto added = ents_.emplace(to_add->get_id(), to_add);
    if (added.second)
    {
        if (!to_add->has<Transform>())
            to_add->add<Transform>(init_params);

        return true;
    }
    return false;
}

Entity * World_Chunk::add(const Entity & copy, const Variant_Map & init_params)
{
    Entity * ent = ns_ctxt.get_world()->create(copy, init_params);
    if (add(ent, init_params))
        return ent;
    bool dest = ns_ctxt.get_world()->destroy(ent);
    assert(dest);
    return nullptr;
}

Entity * World_Chunk::add(const Variant_Map & init_params)
{
    Entity * ent = ns_ctxt.get_world()->create(init_params);
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
    return remove(get(id), remove_from_world);
}

bool World_Chunk::remove(Entity * ent, bool remove_from_world)
{
    if (ent == nullptr)
        return false;

    ent->remove<Transform>();
    bool ret = true;

    if (remove_from_world)
    {
        // When removing from world, will be auto removed from world chunk via on_ent_destroyed slot
        ret = ns_ctxt.get_world()->destroy(ent);
    }
    else
    {
        sizet cnt = ents_.erase(ent->get_id());
        ret = (cnt == 1);
    }
    return ret;
}

void World_Chunk::on_ent_id_change(Tuple2<u32> ids)
{
    Entity * old_ent = get(ids.x);
    if (old_ent != nullptr)
    {
        ents_.erase(old_ent->get_id());
        auto ins = ents_.emplace(ids.y, old_ent);
        assert(ins.second);
    }
}

void World_Chunk::on_ent_destroyed(u32 id)
{
    ents_.erase(id);
}

} // namespace noble_steed
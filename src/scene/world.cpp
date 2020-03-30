#include <noble_steed/scene/world.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/core/context.h>
#include <noble_steed/scene/entity.h>
#include <noble_steed/scene/component.h>

namespace noble_steed
{
World::World() : systems_(), ent_ptrs_(), ent_id_stack_(), ent_current_id_(0)
{}

World::~World()
{}

void World::initialize(const Variant_Map & init_params)
{
    ilog("Initializing world");
    u16 alloc_amount = DEFAULT_ENTITY_ALLOC;
    auto fiter = init_params.find(ENTITY_ALLOC_KEY);
    if (fiter != init_params.end() && fiter->second.is_type<u16>())
        alloc_amount = fiter->second.get_value<u16>();

    ent_ptrs_.reserve(alloc_amount);
}

void World::terminate()
{
    ilog("Terminating world");
    clear_systems();
    clear_entities();
}

void World::clear_systems()
{
    while (systems_.begin() != systems_.end())
        remove_system_(systems_.begin()->first);
}

void World::clear_entities()
{
    while (!ent_ptrs_.empty())
        destroy(ent_ptrs_.back());
}

System * World::add_system_(const rttr::type & sys_type, const Variant_Map & init_params)
{
    u32 id = type_hash(sys_type);
    if (systems_.find(id) != systems_.end())
    {
        wlog("Cannot add system type {} as it already exits", String(sys_type.get_name()));
        return nullptr;
    }
    auto fac = ns_ctxt.get_factory(sys_type);
    System * sys = fac->create_and_cast<System>();
    assert(sys != nullptr);
    sys->initialize(init_params);
    systems_[id] = sys;
    ilog("Added system {} to world", String(sys_type.get_name()));
    return sys;
}

System * World::get_system_(const rttr::type & sys_type)
{
    auto fiter = systems_.find(type_hash(sys_type));
    if (fiter != systems_.end())
        return fiter->second;
    return nullptr;
}

void World::remove_system_(const rttr::type & sys_type)
{
    remove_system_(type_hash(sys_type));
}

void World::remove_system_(u32 type_id)
{
    auto fac = ns_ctxt.get_factory(type_id);
    auto fiter = systems_.find(type_id);
    if (fiter != systems_.end())
    {
        System * sys = fiter->second;
        sys->terminate();
        systems_.erase(fiter);
        ilog("Removed {} system from world", String(sys->get_type().get_name()));
        fac->destroy(sys);
    }
    else
    {
        wlog("Can't remove system because type id {} wasn't found", type_id);
    }
}

Entity * World::create(const Variant_Map & init_params)
{
    Entity * ent = nullptr;
    auto fac = ns_ctxt.get_factory<Entity>();
    ent = fac->create_and_cast<Entity>();
    assert(ent != nullptr);
    add_entity_(ent, init_params);
    return ent;
}

Entity * World::create(const Entity & copy, const Variant_Map & init_params)
{
    Entity * ent = nullptr;
    auto fac = ns_ctxt.get_factory<Entity>();
    ent = fac->create_and_cast<Entity>(copy);
    assert(ent != nullptr);
    add_entity_(ent, init_params);
    return ent;
}

void World::add_entity_(Entity * to_add, const Variant_Map & init_params)
{
    // Assign id to entity - if crashes here then ent is nullptr likely from insufficient memory block (up the size)
    if (!ent_id_stack_.empty())
    {
        to_add->set_id(ent_id_stack_.top());
        ent_id_stack_.pop();
    }
    else
    {
        ++ent_current_id_;
        to_add->set_id(ent_current_id_);
    }

    ent_ptrs_.push_back(to_add);
    auto ret = entity_ids_.emplace(to_add->get_id(), to_add);

    // Should always be true - if an entity isn't given a unique id there is something waaay wacko
    assert(ret.second);
    to_add->owned = true;
    to_add->initialize(init_params);
}

Entity * World::get(u32 id)
{
    auto fiter = entity_ids_.find(id);
    if (fiter != entity_ids_.end())
        return fiter->second;
    return nullptr;
}

bool World::destroy(u32 id)
{
    return destroy(get(id));
}

bool World::destroy(Entity * ent)
{
    if (ent == nullptr)
        return false;

    auto fac = ns_ctxt.get_factory<Entity>();

    ent->terminate();
    sizet rem = entity_ids_.erase(ent->get_id());

    // Find the pointer - copy the last element of the vector to overwrite the pointer's value
    // and decrease the vector size by one
    bool removed_from_vec = false;
    for (sizet i = 0; i < ent_ptrs_.size(); ++i)
    {
        if (ent == ent_ptrs_[i])
        {
            ent_ptrs_[i] = ent_ptrs_[ent_ptrs_.size() - 1];
            removed_from_vec = true;
            break;
        }
    }
    ent_ptrs_.pop_back();

    // Make sure we were able to correctly remove the id from both containers
    assert(removed_from_vec == (rem == 1));

    if (!removed_from_vec)
    {
        wlog("Could not remove {} from world... isn't included in world's memory. Terminated entity and returned without destroying it.",
             ent->get_name());
        return false;
    }

    // Take the id back
    ent_id_stack_.push(ent->get_id());

    fac->destroy(ent);
    return true;
}
} // namespace noble_steed

#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<World>("World");
}

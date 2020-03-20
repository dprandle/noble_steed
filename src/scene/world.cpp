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
    while (systems_.begin() != systems_.end())
        remove_system_(systems_.begin()->first);
    
    while (!ent_ptrs_.empty())
        destroy(ent_ptrs_.back());
}

System * World::add_system_(const rttr::type & sys_type, const Variant_Map & init_params)
{
    if (systems_.find(sys_type.get_id()) != systems_.end())
    {
        wlog("Cannot add system type {} as it already exits", String(sys_type.get_name()));
        return nullptr;
    }
    System_Factory * fac = ns_ctxt.get_factory<System_Factory>(sys_type);
    System * sys = fac->create();
    assert(sys != nullptr);
    sys->initialize(init_params);
    systems_[sys_type.get_id()] = sys;
    ilog("Added system {} to world", String(sys_type.get_name()));
    return sys;
}

System * World::get_system_(const rttr::type & sys_type)
{
    auto fiter = systems_.find(sys_type.get_id());
    if (fiter != systems_.end())
        return fiter->second;
    return nullptr;
}

void World::remove_system_(const rttr::type & sys_type)
{
    remove_system_(sys_type.get_id());
}

void World::remove_system_(u64 type_id)
{
    auto fiter = systems_.find(type_id);
    if (fiter != systems_.end())
    {
        System * sys = fiter->second;
        sys->terminate();
        systems_.erase(fiter);
        ilog("Removed {} system from world", String(sys->get_type().get_name()));
        ns_ctxt.free(sys);
    }
    else
    {
        wlog("Can't remove system because type id {} wasn't found", type_id);
    }
}

Entity * World::create(const Entity * copy)
{
    PoolAllocator * ent_allocator = ns_ctxt.get_entity_allocator();
    sizet ent_size = sizeof(Entity);
    ilog("Allocated {0} bytes for entity", ent_size);
    Entity * ent = static_cast<Entity *>(ent_allocator->Allocate(ent_size));

    // If crashing here means not enough mem for entities - allocate more
    assert(ent != nullptr);

    ent_ptrs_.push_back(ent);
    if (copy != nullptr)
        new (ent) Entity(*copy);
    else
        new (ent) Entity();

    // Assign id to entity
    if (!ent_id_stack_.empty())
    {
        ent->set_id(ent_id_stack_.top());
        ent_id_stack_.pop();
    }
    else
    {
        ++ent_current_id_;
        ent->set_id(ent_current_id_);
    }

    ent->initialize();
    return ent;
}

void World::destroy(Entity * ent)
{
    PoolAllocator * ent_allocator = ns_ctxt.get_entity_allocator();

    ent->terminate();

    // Take the id back
    ent_id_stack_.push(ent->get_id());

    // This should also call the signal to change the comp id
    ent->set_id(0);

    // Calling the destructor of the ent will automatically disconnect its signals so no
    // need to do that here
    ent->~Entity();

    ilog("De-allocated {} bytes for entity {}", sizeof(Entity), ent->get_name());
    ent_allocator->Free(ent);

    if (ent_ptrs_.empty())
    {
        elog("There was no pointer in ent_ptrs_ vector which means the context has lost track of "
             "it somehow");
        return;
    }

    // Find the pointer - copy the last element of the vector to overwrite the pointer's value
    // and decrease the vector size by one
    for (sizet i = 0; i < ent_ptrs_.size(); ++i)
    {
        if (ent == ent_ptrs_[i])
        {
            ent_ptrs_[i] = ent_ptrs_[ent_ptrs_.size() - 1];
            break;
        }
    }
    ent_ptrs_.pop_back();
}
} // namespace noble_steed

#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<World>("World")
        .constructor<>()
        .method("initialize", &World::initialize, registration::public_access)
        .method("terminate", &World::terminate, registration::public_access)
        .method("create", &World::create, registration::public_access)
        .method("destroy", &World::destroy, registration::public_access);
    // .method("log_internal", &System::log_internal, registration::public_access)
    // .property("internal", &System::get_internal, &System::set_internal);
}

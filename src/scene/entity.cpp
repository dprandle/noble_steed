#include <noble_steed/scene/entity.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/core/context.h>
#include <noble_steed/scene/component.h>
#include <PoolAllocator.h>

namespace noble_steed
{
Entity::Entity() : id_(0), owner_id_(0), name_()
{}

Entity::~Entity()
{}

void Entity::initialize()
{
    ilog("Initializing entity with name {} and id {}", name_, id_);
}

void Entity::terminate()
{
    ilog("Terminating enttiy with name {} and id {}", name_, id_);
    while (comps_.begin() != comps_.end())
        destroy_component(comps_.begin()->second);
}

void Entity::set_name(const String & name)
{
    dlog("Renaming entity with id {} from {} to {}", id_, name_, name);
    name_ = name;
}

String Entity::get_name()
{
    return name_;
}

void Entity::set_id(u32 id)
{
    u32 old_id = id_;
    id_ = id;
    if (old_id && (old_id != id_))
    {
        dlog("Re-assigning entity id from {} to {}", old_id, id_);
        id_change(Pair<u32>(old_id, id_));
    }
}

u32 Entity::get_id()
{
    return id_;
}

void Entity::set_owner_id(sizet owner_id)
{
    sizet old_id = owner_id_;
    owner_id_ = owner_id;
    if (old_id && (old_id != owner_id_))
    {
        dlog("Re-assigning entity owner_id from {} to {}", old_id, owner_id_);
        owner_id_change(Pair<sizet>(old_id, owner_id_));
    }
}

sizet Entity::get_owner_id()
{
    return owner_id_;
}

Component * Entity::create_component_(const rttr::type & type)
{
    PoolAllocator * alloc = ns_ctxt.get_comp_allocator(type);
    ilog("Allocating {0} bytes for {1}", type.get_sizeof(), String(type.get_name()));
    Component * comp_ptr = static_cast<Component *>(alloc->Allocate(type.get_sizeof()));
    return comp_ptr;
}

bool Entity::add_component(Component * comp)
{
    rttr::type t = comp->get_derived_info().m_type;
    auto fiter = comps_.emplace(t.get_id(), comp);
    if (fiter.second)
    {
        comp->owner_id_ = id_;
    }
    else
    {
        wlog("Could not add component type {} to entity {} (name: {}) as the component "
             "already exists",
             String(t.get_name()),
             id_,
             name_);
    }

    return fiter.second;
}

Component * Entity::get_component(const rttr::type & type)
{
    auto fiter = comps_.find(type.get_id());
    if (fiter != comps_.end())
        return fiter->second;
    return nullptr;
}

Component * Entity::remove_component(const rttr::type & type)
{
    Component * ret = nullptr;
    auto fiter = comps_.find(type.get_id());
    if (fiter != comps_.end())
    {
        ret = fiter->second;
        fiter = comps_.erase(fiter);
    }
    else
    {
        wlog("Could not remove component type {} from entity {} (name:{}) as that type of component wasn't found",String(type.get_name()),id_, name_);
    }
    return ret;
}

bool Entity::destroy_component(const rttr::type & type)
{
    Component * comp = remove_component(type);
    if (comp == nullptr)
        return false;
    
    comp->terminate();
    comp->~Component();
    PoolAllocator * alloc = ns_ctxt.get_comp_allocator(type);
    ilog("De-allocating {0} bytes for {1}", type.get_sizeof(), String(type.get_name()));
    alloc->Free(comp);
    return true;
}

bool Entity::destroy_component(Component * comp)
{
    return destroy_component(comp->get_derived_info().m_type);
}   

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Entity>("Entity")
        .constructor<>();
        // .property("id_", &Component::initialize, registration::public_access)
        // .property("terminate", &Component::terminate, registration::public_access)
        // .property("owner_id", &Component::owner_id_, registration::private_access);
}

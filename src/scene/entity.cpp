#include <noble_steed/scene/entity.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/core/context.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/scene/component.h>

namespace noble_steed
{
Entity::Entity() : id_(0), name_()
{}

Entity::~Entity()
{}

void Entity::clear()
{
    while (comps_.begin() != comps_.end())
        remove(comps_.begin()->second);
}

void Entity::initialize(const Variant_Map & init_params)
{
    ilog("Initializing entity with name {} and id {}", name_, id_);
}

void Entity::terminate()
{
    ilog("Terminating entity with name {} and id {}", name_, id_);
}

void Entity::set_name(const String & name)
{
    dlog("Renaming entity with id {} from {} to {}", id_, name_, name);
    name_ = name;
}

const String & Entity::get_name()
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

Component * Entity::add(const rttr::type & comp_type, const Variant_Map & init_params)
{
    Component * comp = allocate_comp_(comp_type);
    if (!add_component_(comp, comp_type, init_params))
    {
        deallocate_comp_(comp, comp_type);
        return nullptr;
    }
    return comp;
}

Component * Entity::add(const Component & copy)
{
    // Get derived info is non const likely because of the m_ptr returned with m_type
    rttr::type t = const_cast<Component &>(copy).get_derived_info().m_type;
    Component * comp = allocate_comp_(t, copy);
    if (!add_component_(comp, t, Variant_Map()))
    {
        deallocate_comp_(comp, t);
        return nullptr;
    }
    return comp;
}

Component * Entity::get(const rttr::type & type)
{
    auto fiter = comps_.find(type_hash(type));
    if (fiter != comps_.end())
        return fiter->second;
    return nullptr;
}

Component * Entity::remove_component_(const rttr::type & type)
{
    Component * ret = nullptr;
    auto fiter = comps_.find(type_hash(type));
    if (fiter != comps_.end())
    {
        ret = fiter->second;
        ret->terminate();
        fiter = comps_.erase(fiter);
    }
    else
    {
        wlog("Could not remove component type {} from entity {} (name:{}) as that type of component wasn't found",
             String(type.get_name()),
             id_,
             name_);
    }
    return ret;
}

Component * Entity::allocate_comp_(const rttr::type & type)
{
    auto fac = ns_ctxt.get_factory(type);
    assert(fac != nullptr);
    Component * comp = fac->create_and_cast<Component>();
    assert(comp != nullptr);
    return comp;
}

Component * Entity::allocate_comp_(const rttr::type & type, const Component & copy)
{
    auto fac = ns_ctxt.get_factory(type);
    assert(fac != nullptr);
    Component * comp = fac->create_and_cast<Component>(copy);
    assert(comp != nullptr);
    return comp;
}

bool Entity::add_component_(Component * comp, const rttr::type & comp_type, const Variant_Map & init_params)
{
    auto fiter = comps_.emplace(type_hash(comp_type), comp);
    if (fiter.second)
    {
        comp->owner_id_ = id_;
        comp->initialize(init_params);
    }
    else
    {
        wlog("Could not add component type {} to entity {} (name: {}) as the component "
             "already exists",
             String(comp_type.get_name()),
             id_,
             name_);
    }
    return fiter.second;
}

void Entity::deallocate_comp_(Component * comp, const rttr::type & comp_type)
{
    assert(comp != nullptr);
    auto fac = ns_ctxt.get_factory(comp_type);
    fac->destroy(comp);
}

bool Entity::remove(const rttr::type & type)
{
    Component * comp = remove_component_(type);
    if (comp == nullptr)
        return false;
    deallocate_comp_(comp, type);
    return true;
}

bool Entity::remove(Component * comp)
{
    return remove(comp->get_derived_info().m_type);
}

void Entity::pack_end(JSON_Archive::Direction io_dir)
{
    if (io_dir == JSON_Archive::DIR_IN)
    {
        auto comp_iter = comps_.begin();
        while (comp_iter != comps_.end())
        {
            comp_iter->second->owner_id_ = id_;
            ++comp_iter;
        }
    }
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Entity>("noble_steed::Entity")
        .property("name", &Entity::get_name, &Entity::set_name, registration::public_access)
        .property("id", &Entity::get_id, &Entity::set_id, registration::public_access)
        (
            metadata("NO_SERIALIZE",true)
        )
        .property("components", &Entity::comps_, registration::public_access);
}

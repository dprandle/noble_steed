#include "entity.h"
#include "world.h"
#include "component.h"
#include "../io/logger.h"
#include "../core/context.h"
#include "../core/type_collection_db.h"

namespace noble_steed
{

Entity::Entity() : Context_Obj(), id_(0), name_(), comps_()
{}

Entity::Entity(const Entity &copy) : Context_Obj(copy), id_(0), name_(copy.name_)
{}

Entity &Entity::operator=(Entity rhs)
{
    swap(rhs);
    return *this;
}

void Entity::swap(Entity &rhs)
{
    Context_Obj::swap(rhs);
    std::swap(id_, rhs.id_);
    std::swap(name_, rhs.name_);
    std::swap(comps_, rhs.comps_);
}

Entity::~Entity()
{
    clear();
}

void Entity::clear()
{
    while (comps_.begin() != comps_.end())
        remove(comps_.begin()->second);
}

void Entity::initialize(const Variant_Map &init_params)
{
    ilog("Initializing entity with name {} and id {}", name_, id_);
}

void Entity::terminate()
{
    ilog("Terminating entity with name {} and id {}", name_, id_);
    clear();
}

void Entity::set_name(const String &name)
{
    dlog("Renaming entity with id {} from {} to {}", id_, name_, name);
    name_ = name;
}

const String &Entity::get_name()
{
    return name_;
}

void Entity::set_id(u32 id)
{
    if (id != id_)
    {
        dlog("Trying to re-assign entity id from {} to {}", id_, id);
        bool success = true;
        emit_sig id_change(Tuple2<u32>(id_, id), &success);

        if (success)
        {
            id_ = id;
            auto iter = comps_.begin();
            while (iter != comps_.end())
            {
                iter->second->owner_id_ = id_;
                ++iter;
            }
        }
        else
        {
            dlog("Id change from {} to {} for entity {} was blocked - likely the new id already exists in the world and the entity is owned by the "
                 "world",
                 id_,
                 id,
                 name_);
        }
    }
}

u32 Entity::get_id()
{
    return id_;
}

Component *Entity::get(const type_index &type)
{
    auto fiter = comps_.find(type);
    if (fiter != comps_.end())
        return fiter->second;
    return nullptr;
}

bool Entity::has(const type_index &comp_type)
{
    return get(comp_type) != nullptr;
}

Component *Entity::remove_component_(const type_index &type)
{
    Component *ret = nullptr;
    auto fiter = comps_.find(type);
    if (fiter != comps_.end())
    {
        ret = fiter->second;
        ret->terminate();
        fiter = comps_.erase(fiter);
    }
    else
    {
        wlog("Could not remove component type {} from entity {} (name:{}) as that type of component wasn't found", type.name(), id_, name_);
    }
    return ret;
}

Component *Entity::allocate_comp_(const type_index &type)
{
    return nullptr;
}

Component *Entity::allocate_comp_(const type_index &type, const Component &copy)
{
    return nullptr;
}

bool Entity::add_component_(Component *comp, const type_index &comp_type, const Variant_Map &init_params)
{
    auto fiter = comps_.emplace(comp_type, comp);
    if (fiter.second)
    {
        comp->owner_id_ = id_;
        comp->initialize(init_params);
    }
    else
    {
        wlog("Could not add component type {} to entity {} (name: {}) as the component "
             "already exists",
             comp_type.name(),
             id_,
             name_);
    }
    return fiter.second;
}

void Entity::deallocate_comp_(Component *comp, const type_index &comp_type)
{}

bool Entity::remove(const type_index &type)
{
    return true;
}

bool Entity::remove(Component *comp)
{
    return remove(typeid(*comp));
}

} // namespace noble_steed
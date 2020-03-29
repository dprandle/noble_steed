#pragma once

#include <noble_steed/container/vector.h>
#include <noble_steed/container/stack.h>
#include <noble_steed/core/system.h>

class PoolAllocator;

namespace noble_steed
{
class Component;
class Entity;
class System;

class World
{
    RTTR_ENABLE()
  public:
    World();

    ~World();

    void initialize(const Variant_Map & init_params = Variant_Map());

    void terminate();

    template<class T>
    T * add_system(const Variant_Map & init_params = Variant_Map())
    {
        rttr::type t = rttr::type::get<T>();
        return static_cast<T*>(add_system_(t,init_params));
    }

    template<class T>
    T * get_system()
    {
        rttr::type t = rttr::type::get<T>();
        return static_cast<T*>(get_system_(t));
    }

    template<class T>
    void remove_system()
    {
        rttr::type t = rttr::type::get<T>();
        remove_system_(t);
    }

    Entity * create(const Entity * copy = nullptr, const Variant_Map & init_params=Variant_Map());

    Entity * get(u32 id);

    bool destroy(Entity * ent);

    bool destroy(u32 id);

  private:
    System * add_system_(const rttr::type & sys_typ,const Variant_Map & init_params);

    System * get_system_(const rttr::type & sys_type);

    void remove_system_(const rttr::type & sys_type);

    void remove_system_(u32 type_id);

    Hash_Map<u64, System*> systems_;

    Hash_Map<u32, Entity*> entity_ids_;

    Vector<Entity*> ent_ptrs_;

    Stack<u32> ent_id_stack_;

    u32 ent_current_id_;
};
} // namespace noble_steed
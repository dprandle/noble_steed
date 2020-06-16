#pragma once

#include <noble_steed/container/vector.h>
#include <noble_steed/container/stack.h>
#include <noble_steed/core/system.h>

class PoolAllocator;

#define ns_world ns_ctxt.get_world()

namespace noble_steed
{
class Component;
class Entity;
class System;

class World
{
    friend class Context;
    friend Variant construct_factory_type(u32 type_id);

  public:
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

    void clear_systems();

    void clear_entities();

    Entity * create(const Variant_Map & init_params=Variant_Map());

    Entity * create(const Entity & copy, const Variant_Map & init_params=Variant_Map());

    bool contains(u32 id);

    Entity * get(u32 id);

    bool destroy(Entity * ent);

    bool destroy(u32 id);

    // Signals
    Signal<u32> entity_destroyed;

    Signal<Tuple2<u32>> entity_id_change;

  private:
    World();

    World(const World &) = delete;

    World & operator=(const World &) = delete;

    ~World();

    void add_default_systems(const Variant_Map & init_params);

    void rebuild_available_id_stack_();
    
    void add_entity_(Entity * to_add, const Variant_Map & init_params);
    
    System * add_system_(const rttr::type & sys_typ,const Variant_Map & init_params);

    System * get_system_(const rttr::type & sys_type);

    void remove_system_(const rttr::type & sys_type);

    void remove_system_(u32 type_id);

    Hash_Map<u64, System*> systems_;

    Hash_Map<u32, Entity*> entity_ids_;

    Vector<Entity*> ent_ptrs_;

    Stack<u32> ent_id_stack_;

    u32 ent_current_id_;

    SLOT_OBJECT
};
} // namespace noble_steed
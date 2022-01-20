#pragma once

#include "../container/vector.h"
#include "../container/stack.h"
#include "../container/tuple.h"
#include "../core/system.h"

class Pool_Allocator;

#define ns_world ns_ctxt.get_world()

/// Init params key to override the default entity allocation amount

namespace noble_steed
{
const String ENTITY_ALLOC_KEY = "Entity_Alloc";
class Component;
class Entity;
class System;

class World : public Context_Obj
{
    friend class Context;
    friend Variant construct_factory_type(u32 type_id);

  public:
    void initialize(const Variant_Map & init_params = Variant_Map());

    void terminate();

    template<class T>
    T * add_system(const Variant_Map & init_params = Variant_Map())
    {
        type_index t = typeid(T);
        return static_cast<T*>(add_system_(t,init_params));
    }

    template<class T>
    T * get_system()
    {
        type_index t = typeid(T);
        return static_cast<T*>(get_system_(t));
    }

    template<class T>
    void remove_system()
    {
        type_index t = typeid(T);
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

    void handle_entity_packed_in(Event & ev);

    void add_default_systems(const Variant_Map & init_params);

    void rebuild_available_id_stack_();
    
    void add_entity_(Entity * to_add, const Variant_Map & init_params);
    
    System * add_system_(const type_index & sys_typ,const Variant_Map & init_params);

    System * get_system_(const type_index & sys_type);

    void remove_system_(const type_index & sys_type);

    Hash_Map<type_index, System*> systems_;

    Hash_Map<u32, Entity*> entity_ids_;

    Vector<Entity*> ent_ptrs_;

    Stack<u32> ent_id_stack_;

    u32 ent_current_id_;

    SLOT_OBJECT
};
} // namespace noble_steed
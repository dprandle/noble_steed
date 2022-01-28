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
    void initialize(const Variant_Map &init_params = Variant_Map());

    void terminate();

    void clear_entities();

    Entity *create(const Variant_Map &init_params = Variant_Map());

    Entity *create(const Entity &copy, const Variant_Map &init_params = Variant_Map());

    bool contains(u32 id);

    Entity *get(u32 id);

    bool destroy(Entity *ent);

    bool destroy(u32 id);

    // Signals
    Signal<u32> entity_destroyed;

    Signal<Tuple2<u32>> entity_id_change;

  private:
    World();

    World(const World &) = delete;

    World &operator=(const World &) = delete;

    ~World();

    void handle_entity_packed_in(Event &ev);

    void rebuild_available_id_stack_();

    void add_entity_(Entity *to_add, const Variant_Map &init_params);

    Hash_Map<u32, Entity *> entity_ids_;

    Vector<Entity *> ent_ptrs_;

    Stack<u32> ent_id_stack_;

    u32 ent_current_id_;

    SLOT_OBJECT
};

} // namespace noble_steed
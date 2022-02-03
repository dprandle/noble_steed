#pragma once

#include "../core/basic_types.h"
#include "../core/resource.h"
#include "../container/tuple.h"

namespace noble_steed
{

class Entity;

class World_Chunk : public Resource
{
    template<class T>
    friend class Pool_Factory;

  public:
    World_Chunk();

    World_Chunk(const World_Chunk &copy);

    ~World_Chunk();

    void initialize(const Variant_Map &init_params);

    void terminate();

    bool add(Entity *to_add, const Variant_Map &init_params = Variant_Map());

    Entity *add(const Variant_Map &init_params = Variant_Map());

    Entity *add(const Entity &copy, const Variant_Map &init_params = Variant_Map());

    void clear(bool remove_entities_from_world);

    Entity *get(u32 id);

    bool remove(u32 id, bool remove_from_world);

    bool remove(Entity *ent, bool remove_from_world);

    World_Chunk &operator=(World_Chunk rhs);

  protected:
    void swap(World_Chunk &rhs);

  private:
    void on_ent_id_change(Tuple2<u32> ids);

    void on_ent_destroyed(u32 id);

    vector<Entity *> ents_ptrs_;
    hash_map<u32, Entity *> ents_;
};

} // namespace noble_steed
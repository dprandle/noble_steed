#pragma once

#include "../core/common.h"
#include "../core/tuple.h"
#include "../core/resource.h"

namespace noble_steed
{
class Entity;

class World_Chunk : public Resource
{
    template<class T> friend class Pool_Factory;

  public:
    World_Chunk();

    World_Chunk(const World_Chunk & copy);

    ~World_Chunk();

    void initialize(const Variant_Hash & init_params);

    void terminate();

    bool add(Entity * to_add, const Variant_Hash & init_params = Variant_Hash());

    Entity * add(const Variant_Hash & init_params = Variant_Hash());

    Entity * add(const Entity & copy, const Variant_Hash & init_params = Variant_Hash());

    void clear(bool remove_entities_from_world);

    Entity * get(u32 id);

    bool remove(u32 id, bool remove_from_world);

    bool remove(Entity * ent, bool remove_from_world);

    World_Chunk & operator=(World_Chunk rhs);

  protected:
    void swap(World_Chunk & rhs);

    void pack_begin(JSON_Archive::Direction io_dir);

    void pack_end(JSON_Archive::Direction io_dir);

  private:
    void on_ent_id_change(Tuple2<u32> ids);

    void on_ent_destroyed(u32 id);

    Vector<Entity *> ents_ptrs_;
    Hash_Map<u32, Entity *> ents_;

    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(Resource)
};
} // namespace noble_steed
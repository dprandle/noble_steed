#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/resource.h>

namespace noble_steed
{
class Entity;

class World_Chunk : public Resource
{
    RTTR_ENABLE(Resource)
  public:
    World_Chunk();

    ~World_Chunk();

    bool add(Entity * to_add, const Variant_Map & init_params=Variant_Map());

    Entity * add(const Variant_Map & init_params=Variant_Map());

    Entity * get(u64 id);

    bool remove(u64 id, bool remove_from_world=true);

    bool remove(Entity * ent,bool remove_from_world=true);

    void initialize(const Variant_Map & init_params);

    void terminate();

    void pack_unpack(JSON_Archive & ar);

  private:
    
    Hash_Map<u64,Entity*> ents_;
};
} // namespace noble_steed
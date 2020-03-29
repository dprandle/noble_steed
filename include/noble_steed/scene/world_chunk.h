#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/resource.h>

namespace noble_steed
{
class Entity;

class World_Chunk : public Resource
{
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(Resource)
  public:
    World_Chunk();

    ~World_Chunk();

    bool add(Entity * to_add, const Variant_Map & init_params=Variant_Map());

    Entity * add(const Variant_Map & init_params=Variant_Map());

    void clear();

    Entity * get(u32 id);

    bool remove(u32 id, bool remove_from_world=true);

    bool remove(Entity * ent,bool remove_from_world=true);

    void initialize(const Variant_Map & init_params);

    void terminate();

protected:
    void pack_begin(JSON_Archive::Direction io_dir);
    void pack_end(JSON_Archive::Direction io_dir);

  private:
    Vector<Entity*> ents_ptrs_;
    Hash_Map<u32,Entity*> ents_;
};
} // namespace noble_steed
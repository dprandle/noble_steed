#include "core/context.h"
#include "scene/transform.h"
#include "scene/entity.h"
#include "scene/world.h"
#include "core/resource.h"
#include "core/resource_cache.h"
#include "scene/world_chunk.h"

using namespace noble_steed;

// struct silly_system : public System
// {
//   public:
//     SLOT_OBJECT
//     int poop;
//     int bla;

//     void received_id_change(u64 old_id, u64 new_id)
//     {
//         dlog("Received id change from {} tp {}", old_id, new_id);
//     }
// };

// class silly_resource : public Resource
// {
//   public:
//     int scooby;
//     int dooby;
// };

int main()
{
    // Context ctxt;
    // Variant_Map vm;

    // ctxt.initialize(vm);
    // ctxt.register_system_type<silly_system>(vm);
    // ctxt.register_resource_type<silly_resource>(".silly", vm);

    // World * world = ctxt.get_world();
    // World_Chunk * wc = ctxt.get_resource_cache()->add<World_Chunk>("maps/basic");

    // Entity * ent = world->create();
    // ent->set_name("Test");
    // wc->add(ent);

    // // Entity * ent2 = wc->add();

    // //wc->save();

    // wc->load();

    // wc->save("poppy_seed.bbworld");

    // ctxt.terminate();
    return 0;
}
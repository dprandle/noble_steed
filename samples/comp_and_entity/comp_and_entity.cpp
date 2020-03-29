#include <noble_steed/core/context.h>
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/entity.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/core/resource.h>
#include <noble_steed/core/resource_cache.h>
#include <noble_steed/scene/world_chunk.h>

using namespace noble_steed;

struct silly_system : public System
{
    RTTR_ENABLE(System)
  public:
    SLOT_OBJECT
    int poop;
    int bla;

    void received_id_change(u64 old_id, u64 new_id)
    {
        dlog("Received id change from {} tp {}", old_id, new_id);
    }
};

class silly_resource : public Resource
{
    RTTR_ENABLE(Resource)
  public:
    int scooby;
    int dooby;

    // virtual void pack_unpack(JSON_Archive & ar)
    // {
    //     dlog("Calling in derived");
    //     Resource::pack_unpack(ar);
    // }
};

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<silly_resource>("silly_resource")
        .constructor<>()
        .property("scooby", &silly_resource::scooby, registration::public_access)
        .property("dooby", &silly_resource::dooby, registration::public_access);
}

int main()
{
    Context ctxt;
    Variant_Map vm;

    Variant v;
    Hash_Map<int, int> hm;
    v = hm;

    ctxt.initialize(vm);
    ctxt.register_system_type<silly_system>(vm);
    ctxt.register_resource_type<silly_resource>(".silly", vm);

    World * world = ctxt.get_world();
    World_Chunk * wc = ctxt.get_resource_cache()->add<World_Chunk>("maps/basic");

    Entity * ent = world->create();
    ent->set_name("Test");
    wc->add(ent);

    // Entity * ent2 = wc->add();

    //wc->save();

    wc->load();

    wc->save("poppy_seed.bbworld");

    ctxt.terminate();
    return 0;
}
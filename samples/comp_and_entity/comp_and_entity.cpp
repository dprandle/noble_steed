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
        dlog("Received id change from {} tp {}",old_id,new_id);
    }
};

class silly_resource : public Resource
{
    RTTR_ENABLE(Resource)
    public:
    int scooby;
    int dooby;

    virtual void pack_unpack(JSON_Archive & ar)
    {
        dlog("Calling in derived");
        Resource::pack_unpack(ar);
    }
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
    Hash_Map<int,int> hm;
    v = hm;
    
    ctxt.initialize(vm);
    ctxt.register_system_type<silly_system>(vm);
    ctxt.register_resource_type<silly_resource>(".silly",vm);

    World * world = ctxt.get_world();
    Entity * ent = world->create();
    ent->set_name("BlaBla");
    
    world->add_system<silly_system>();

    silly_resource * res = ctxt.get_resource_cache()->add<silly_resource>("Scooby.mdl");

    res->set_display_name("Poopity poo");
    res->scooby = 1;
    res->save();

    silly_resource * cpy = ctxt.get_resource_cache()->add<silly_resource>(*res,"Copy");
    cpy->save();

    silly_resource * finally = ctxt.get_resource_cache()->add<silly_resource>("Finally/There");
    finally->set_name("Scooby.mdl");
    finally->save();

    World_Chunk * wc = ctxt.get_resource_cache()->add<World_Chunk>("funky_town/my_blue_panet");
    wc->add(ent);

    Entity * ent2 = wc->add();

    wc->save();

    ctxt.terminate();
    return 0;
}
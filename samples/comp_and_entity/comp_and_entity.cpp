#include <noble_steed/core/context.h>
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/entity.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/core/resource.h>

using namespace noble_steed;

struct silly_system : public System
{
    RTTR_ENABLE(System)
    int poop;
    int bla;
};

int main()
{
    Context ctxt;
    Variant_Map vm;
    
    ctxt.initialize(vm);
    ctxt.register_system_type<silly_system>(vm);
    ctxt.register_component_type<Transform>(vm);

    World * world = ctxt.get_world();
    Entity * ent = world->create();
    ent->set_name("BlaBla");
    world->add_system<silly_system>();
    world->add_system<silly_system>();
    world->remove_system<silly_system>();
    world->remove_system<silly_system>();
    world->add_system<silly_system>();


    Transform * tform = ent->add<Transform>();
    ent->remove<Transform>();
    tform = ent->add<Transform>();
    ent->add<Transform>();
    ent->remove(tform);
    ctxt.terminate();
    return 0;
}
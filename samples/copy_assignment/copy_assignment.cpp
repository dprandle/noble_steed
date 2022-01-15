#include <noble_steed/core/context.h>
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/entity.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/core/resource.h>
#include <noble_steed/core/resource_cache.h>
#include <noble_steed/scene/world_chunk.h>

using namespace noble_steed;

int main()
{
    Context ctxt;
    Variant_Map vm;

    ctxt.initialize(vm);
    
    World * world = ctxt.get_world();
    World_Chunk * wc = ctxt.get_resource_cache()->add<World_Chunk>("maps/basic");
    Entity * ent = world->create();
    ent->set_name("Test");


    // Add to world chunk - first one should be good, the other two should fail - should add tform to entity

    // Copy assignment - ent should have id of 0 and add transform with owner id of 0
    Entity new_ent = *ent;
    wc->add(ent);
    new_ent.set_name("Test 2");

    // Copy construct - ent should have id of 0 and add transform with owner id of 0
    Entity new_ent_2(new_ent);
    new_ent_2.set_name("Test 3");

    // Both of these should fail
    wc->add(&new_ent);
    wc->add(&new_ent_2);

    dlog(ent->to_json());
    dlog(new_ent.to_json());
    dlog(new_ent_2.to_json());
    
    // This should add a non-owned world chunk with all the same entities as wc - all these new entities and components will
    // be context owned despited wc not being context owned - this is fine as long as they are removed
    World_Chunk wc2 = *wc;
    World_Chunk wc3(wc2);
    World_Chunk * wc4 = ctxt.get_resource_cache()->add(wc2);

    auto ent4 = wc3.add();
    auto ent5 = wc2.add();
    ent4->set_name("Blabla");
    ent5->set_name("BlaBla2");
    ent5->set_id(100);
    ent4->set_id(1);
    auto rc = ctxt.get_resource_cache();

    wc->load();
    wc2.load();

    wc->save("poppy_seed.bbworld");
    wc2.save("poppy_seed");
    wc3.save();
    wc4->save();

    ctxt.terminate();
    
    return 0;

}
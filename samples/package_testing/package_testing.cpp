#include <noble_steed/core/context.h>
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/entity.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/core/resource.h>
#include <noble_steed/core/resource_cache.h>
#include <noble_steed/scene/world_chunk.h>

using namespace noble_steed;

class silly_resource_1 : public Resource
{
    RTTR_ENABLE(Resource)
  public:
    int scooby;
    int dooby;
};

class silly_resource_2 : public Resource
{
    RTTR_ENABLE(Resource)
  public:
    int channel;
    int tooby;
};

class silly_resource_3 : public Resource
{
    RTTR_ENABLE(Resource)
  public:
    int big_fat_lady;
    int channel_80;
};

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<silly_resource_1>("silly_resource_1")
        .property("scooby", &silly_resource_1::scooby)
        .property("dooby", &silly_resource_1::dooby);

    registration::class_<silly_resource_2>("silly_resource_2")
        .property("scooby", &silly_resource_2::channel)
        .property("dooby", &silly_resource_2::tooby);

    registration::class_<silly_resource_3>("silly_resource_3")
        .property("scooby", &silly_resource_3::big_fat_lady)
        .property("dooby", &silly_resource_3::channel_80);
}

int main()
{
    Context ctxt;
    Variant_Map vm;
    String current_package = "data/best_package_ever";
    Vector<String> packages = {current_package};
    vm[IP_PACKAGE_DIRS_KEY] = packages;
    vm[IP_CURRENT_PACKAGE_KEY] = current_package;

    ctxt.register_resource_type<silly_resource_1>("res1",vm);
    ctxt.register_resource_type<silly_resource_2>("res2",vm);
    ctxt.register_resource_type<silly_resource_3>("res3",vm);
    ctxt.initialize(vm);

    Resource_Cache * rc = ctxt.get_resource_cache();

    // auto sr1 = rc->add<silly_resource_1>("silly_1");
    // auto sr2 = rc->add<silly_resource_2>("silly_2");
    // auto sr3 = rc->add<silly_resource_3>("silly_3");

    // sr1->scooby = 5;
    // sr1->dooby = 6;
    // sr2->channel = 7;
    // sr2->tooby = 8;
    // sr3->big_fat_lady = 9;
    // sr3->channel_80 = 10;

    // rc->save_all();



    ctxt.terminate();
    return 0;

}
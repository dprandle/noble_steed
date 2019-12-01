#include <noble_steed/core/context.h>
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/entity.h>

using namespace noble_steed;

int main()
{
    Context ctxt;
    ctxt.initialize();
    Entity * sample = ns_ctxt.create_entity();
    sample->set_name("Daniel");
    Transform * tform = sample->create_component<Transform>();
    ctxt.terminate();
    return 0;
}
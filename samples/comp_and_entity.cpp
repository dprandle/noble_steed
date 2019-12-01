#include <noble_steed/core/context.h>
#include <noble_steed/scene/transform.h>

using namespace noble_steed;

int main()
{
    Context ctxt;
    ctxt.initialize();
    Transform * tform = ns_ctxt.create_component<Transform>();
    Transform * tform2 = ns_ctxt.create_component<Transform>();
//    ns_ctxt.destroy_component(tform);
    Transform * tform3 = ns_ctxt.create_component<Transform>();
    // ns_ctxt.destroy_component(tform2);
    // ns_ctxt.destroy_component(tform3);
    ctxt.terminate();
    return 0;
}
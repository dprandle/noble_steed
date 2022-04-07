#include "window.h"
#include "nsmemory.h"

using namespace noble_steed;

struct example_item
{
    int item_int {5};
    double item_dbl {7.4};
    char item_char{'b'};
    u8 item_byte{0x44};
};

int main()
{
    window_create_params cp {};
    cp.resolution = {800, 600};
    copy_str(&cp.title.str, "My Window");
    cp.win_flags = window_flags::VISIBLE | window_flags::DECORATED;
    
    mem_store mem {};
    mem_pool_alloc<example_item>(&mem, 6);
    mem_store_init(&mem);

    auto item = ns_new<example_item>(&mem);
    auto item2 = ns_new<example_item>(&mem);
    auto item3 = ns_new<example_item>(&mem);
    auto item4 = ns_new<example_item>(&mem);
    auto item5 = ns_new<example_item>(&mem);

    ns_free(&mem, item3);
    item3 = ns_new<example_item>(&mem);
    ns_free(&mem, item);
    ns_free(&mem, item2);
    item = ns_new<example_item>(&mem);
    item2 = ns_new<example_item>(&mem);    

    
    window w = create_window(cp);


    mem_store_term(&mem);
    mem_store_free(&mem);
}

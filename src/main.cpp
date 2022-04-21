
#include "platform.h"

using namespace noble_steed;

int main()
{
    platform_ctxt pform {};
    window_create_params cp;
    cp.resolution = {800,600};
    copy_str(&cp.title, "Build And Battle");
    cp.win_flags = window_flags::VISIBLE | window_flags::DECORATED;

    platform_init(&pform, cp);
    platform_exec(&pform);
    platform_term(&pform);
}
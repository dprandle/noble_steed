
#include "platform.h"

using namespace noble_steed;

int main()
{
    platform_ctxt pform {};
    window_create_params cp;
    cp.resolution = {1920,1080};
    copy_str(&cp.title, "Build And Battle");
    cp.win_flags = window_flags::VISIBLE | window_flags::DECORATED | window_flags::SCALE_TO_MONITOR;

    platform_init(&pform, cp);
    platform_exec(&pform);
    platform_term(&pform);
}
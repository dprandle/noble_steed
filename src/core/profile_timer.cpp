#include "profile_timer.h"

namespace noble_steed
{
void profile_timer::reset()
{
    prev = profile_clock::now();
    cur = profile_clock::now();
}

u64 profile_timer::elapsed()
{
    cur = profile_clock::now();
    u64 ret = std::chrono::duration_cast<profile_duration>(cur - prev).count();
    prev = cur;
    return ret;
}
}
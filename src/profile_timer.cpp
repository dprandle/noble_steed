#include "profile_timer.h"

namespace noble_steed
{
u64 pt_update_elapsed(profile_timepoints *ptimer)
{
    ptimer->cur = profile_clock::now();
    u64 ret = std::chrono::duration_cast<profile_duration>(ptimer->cur - ptimer->prev).count();
    return ret;
}

void pt_start(profile_timepoints *ptimer)
{
    ptimer->prev = profile_clock::now();
    ptimer->cur = profile_clock::now();
}

u64 pt_restart_elapsed(profile_timepoints *ptimer)
{
    u64 ret = pt_update_elapsed(ptimer);
    ptimer->prev = ptimer->cur;
    return ret;
}
} // namespace noble_steed
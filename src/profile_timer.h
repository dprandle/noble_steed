#pragma once

#include <chrono>
#include "basic_types.h"

namespace noble_steed
{

using profile_clock = std::chrono::high_resolution_clock;
using profile_duration = std::chrono::microseconds;
using profile_timepoint = std::chrono::time_point<profile_clock>;

struct profile_timepoints
{
    profile_timepoint cur;
    profile_timepoint prev;
};

void pt_start(profile_timepoints * ptimer);

/// Update cur timepoint and return us since prev after updating
u64 pt_update_elapsed(profile_timepoints * ptimer);

/// Update cur timepoint and return us since prev, then update prev to cur
u64 pt_restart_elapsed(profile_timepoints * ptimer);
}
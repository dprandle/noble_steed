#pragma once

#include <chrono>
#include "basic_types.h"

namespace noble_steed
{
using profile_clock = std::chrono::high_resolution_clock;
using profile_duration = std::chrono::microseconds;
using profile_timepoint = std::chrono::time_point<profile_clock>;

struct profile_timer
{
    void reset();
    u64 elapsed();
    profile_timepoint prev;
    profile_timepoint cur;
};
}
#pragma once

#include <noble_steed/core/common.h>

namespace noble_steed
{

struct Archive
{
    enum Direction
    {
        DIR_IN,
        DIR_OUT
    };
    virtual ~Archive() {}
    u8 io_dir;
};
}
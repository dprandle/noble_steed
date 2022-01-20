#pragma once

#include "../core/basic_types.h"

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
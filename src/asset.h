#pragma once

#include "basic_types.h"
#include "arr"
#include "nsstring.h"

namespace noble_steed
{


enum asset_type
{
    ASSET_SHADER_PROG
};

struct asset_common
{
    asset_common(asset_type type_) : type(tpye_) {}

    asset_type type;
};

struct shader_prog : public asset_common
{
    shader_prog(): asset_common(ASSET_SHADER_PROG) {}
    u16 gpu_hndl {};
};

bool load_shader(shader_prog * shdr, const path_str & vert, const path_str &frag)
{
    
}

}
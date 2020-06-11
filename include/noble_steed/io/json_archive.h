#pragma once

#include <noble_steed/io/archive.h>
#include <rttr/type>
#include <ostream>
#include <noble_steed/container/string.h>

namespace noble_steed
{
struct JSON_Archive : public Archive
{
    String json_str;
};

#define JSON_PACKABLE

void pack_unpack(JSON_Archive & ar, rttr::instance obj);

class JSON_Packable
{
    RTTR_ENABLE()
    JSON_PACKABLE
};

} // namespace noble_steed
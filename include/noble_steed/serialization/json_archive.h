#pragma once

#include <noble_steed/serialization/archive.h>
#include <rttr/type>

namespace noble_steed
{
struct JSON_Archive : public Archive
{
    String json_str;
};

void pack_unpack(JSON_Archive & ar, rttr::instance obj);

} // namespace noble_steed
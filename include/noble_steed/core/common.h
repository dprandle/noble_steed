#pragma once

#include <noble_steed/container/string.h>
#include <noble_steed/container/hash_map.h>
#include <rttr/type>

namespace noble_steed
{
using Variant_Map = Hash_Map<String, rttr::variant>;

const int KB_SIZE = 1024 * 8;
const int MB_SIZE = 1024 * KB_SIZE;
} // namespace noble_steed
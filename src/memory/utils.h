#pragma once
#include "../core/basic_types.h"

namespace noble_steed
{
namespace mem_util
{

sizet calc_padding(sizet base_addr, sizet alignment);
sizet calc_padding_with_header(sizet base_addr, sizet alignment, sizet header_size);

} // namespace mem_util
} // namespace noble_steed
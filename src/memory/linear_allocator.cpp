#include <stdlib.h>
#include <cassert>
#include <algorithm>

#include "linear_allocator.h"
#include "utils.h"

namespace noble_steed
{
Linear_Allocator::Linear_Allocator(sizet total_size, Mem_Resource_Base *upstream) : Allocator(total_size, upstream), _offset(0)
{}

Linear_Allocator::Linear_Allocator(sizet total_size) : Allocator(total_size, get_default_resource()), _offset(0)
{}

Linear_Allocator::~Linear_Allocator()
{}

void *Linear_Allocator::do_allocate(sizet size, sizet alignment)
{
    sizet padding = 0;
    sizet current_addr = (sizet)_start_ptr + _offset;

    // Alignment is required. Find the next aligned memory address and update offset
    if (alignment != 0 && _offset % alignment != 0)
        padding = util::calc_padding(current_addr, alignment);

    if (_offset + padding + size > _total_size)
        return nullptr;

    _offset += padding;
    sizet next_addr = current_addr + padding;
    _offset += size;
    _used = _offset;
    _peak = std::max(_peak, _used);
    return (void *)next_addr;
}

void Linear_Allocator::do_deallocate(void *, sizet, sizet)
{
    // NO OP
}

void Linear_Allocator::do_reset()
{
    _offset = 0;
}

} // namespace noble_steed
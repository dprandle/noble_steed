#include <stdlib.h>
#include <cassert>
#include <algorithm>

#include "linear_allocator.h"
#include "utils.h"

namespace noble_steed
{

Linear_Allocator::Linear_Allocator(sizet total_size) : Allocator(total_size)
{}

void Linear_Allocator::init()
{
    if (_start_ptr != nullptr)
    {
        free(_start_ptr);
    }
    _start_ptr = malloc(_total_size);
    _offset = 0;
}

Linear_Allocator::~Linear_Allocator()
{
    free(_start_ptr);
    _start_ptr = nullptr;
}

void *Linear_Allocator::allocate(sizet size, sizet alignment)
{
    sizet padding = 0;
    sizet current_addr = (sizet)_start_ptr + _offset;

    if (alignment != 0 && _offset % alignment != 0)
    {
        // Alignment is required. Find the next aligned memory address and update offset
        padding = mem_util::calc_padding(current_addr, alignment);
    }

    if (_offset + padding + size > _total_size)
    {
        return nullptr;
    }

    _offset += padding;
    sizet next_addr = current_addr + padding;
    _offset += size;
    _used = _offset;
    _peak = std::max(_peak, _used);
    return (void *)next_addr;
}

void Linear_Allocator::free(void *ptr)
{
    assert(false && "Use reset() method");
}

void Linear_Allocator::reset()
{
    _offset = 0;
    _used = 0;
    _peak = 0;
}

} // namespace noble_steed
#include <stdlib.h>
#include <algorithm>

#include "stack_allocator.h"
#include "utils.h"

namespace noble_steed::memory
{

Stack_Allocator::Stack_Allocator(sizet total_size, Mem_Resource_Base *upstream): Allocator(total_size, upstream), _offset(0)
{}

Stack_Allocator::Stack_Allocator(sizet total_size) : Allocator(total_size, get_default_resource()), _offset(0)
{}

Stack_Allocator::~Stack_Allocator()
{}

void *Stack_Allocator::do_allocate(sizet size, sizet alignment)
{
    sizet current_addr = (sizet)_start_ptr + _offset;

    char padding = util::calc_padding_with_header(current_addr, alignment, sizeof(Alloc_Header));

    if (_offset + padding + size > _total_size)
        return nullptr;

    _offset += padding;

    sizet next_addr = current_addr + padding;
    sizet header_addr = next_addr - sizeof(Alloc_Header);
    Alloc_Header alloc_header{padding};
    Alloc_Header *header_ptr = (Alloc_Header *)header_addr;
    header_ptr = &alloc_header;

    _offset += size;
    _used = _offset;
    _peak = std::max(_peak, _used);

    return (void *)next_addr;
}

void Stack_Allocator::do_deallocate(void *ptr, sizet, sizet)
{
    // Move offset back to clear address
    sizet current_addr = (sizet)ptr;
    sizet header_addr = current_addr - sizeof(Alloc_Header);
    Alloc_Header *alloc_header{(Alloc_Header *)header_addr};

    _offset = current_addr - alloc_header->padding - (sizet)_start_ptr;
    _used = _offset;
}

void Stack_Allocator::do_reset()
{
    _offset = 0;
}

} // namespace noble_steed::memory
#include <assert.h>
#include <algorithm>

#include "pool_allocator.h"

namespace noble_steed
{

Pool_Allocator::Pool_Allocator(sizet total_size, sizet chunk_size, Mem_Resource_Base *upstream)
    : Allocator(total_size, upstream), _chunk_size(chunk_size)
{
    assert(_chunk_size >= 8 && "Chunk size must be greater or equal to 8");
    assert(total_size % _chunk_size == 0 && "Total Size must be a multiple of Chunk Size");
    do_reset();
}

Pool_Allocator::Pool_Allocator(sizet total_size, sizet chunk_size) : Allocator(total_size, get_default_resource()), _chunk_size(chunk_size)
{
    assert(_chunk_size >= 8 && "Chunk size must be greater or equal to 8");
    assert(total_size % _chunk_size == 0 && "Total Size must be a multiple of Chunk Size");
    do_reset();
}

Pool_Allocator::~Pool_Allocator()
{}

void *Pool_Allocator::do_allocate(sizet size, sizet)
{
    assert(size == _chunk_size && "Allocation size must be even multiple of chunk size");
    Node *free_pos = _free_list.pop();
    assert(free_pos && "The pool allocator is full");
    _used += _chunk_size;
    _peak = std::max(_peak, _used);
    return (void *)free_pos;
}

void Pool_Allocator::do_deallocate(void *ptr, sizet bytes, sizet)
{
    assert(bytes == _chunk_size && "Byte size does not match pool allocator chunk size");
    _used -= _chunk_size;
    _free_list.push((Node *)ptr);
}

void Pool_Allocator::do_reset()
{
    // Create a linked-list with all free positions
    int nchunks = _total_size / _chunk_size;
    for (int i = 0; i < nchunks; ++i)
    {
        sizet address = (sizet)_start_ptr + i * _chunk_size;
        _free_list.push((Node *)address);
    }
}

} // namespace noble_steed
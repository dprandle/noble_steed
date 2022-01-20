#include <assert.h>
#include <algorithm>

#include "pool_allocator.h"

namespace noble_steed
{

Pool_Allocator::Pool_Allocator(sizet total_size, sizet chunk_size) : Allocator(total_size), _chunk_size(chunk_size)
{
    assert(_chunk_size >= 8 && "Chunk size must be greater or equal to 8");
    assert(total_size % _chunk_size == 0 && "Total Size must be a multiple of Chunk Size");
}

void Pool_Allocator::init()
{
    _start_ptr = malloc(_total_size);
    reset();
}

Pool_Allocator::~Pool_Allocator()
{
    free(_start_ptr);
}

void *Pool_Allocator::allocate(sizet alloc_size, sizet)
{
    assert(alloc_size == _chunk_size && "Allocation size must be equal to chunk size");

    Node *free_pos = _free_list.pop();

    assert(free_pos != nullptr && "The pool allocator is full");

    _used += _chunk_size;
    _peak = std::max(_peak, _used);
    return (void *)free_pos;
}

void Pool_Allocator::free(void *ptr)
{
    _used -= _chunk_size;
    _free_list.push((Node *)ptr);
}

void Pool_Allocator::reset()
{
    _used = 0;
    _peak = 0;

    // Create a linked-list with all free positions
    int nchunks = _total_size / _chunk_size;
    for (int i = 0; i < nchunks; ++i)
    {
        sizet address = (sizet)_start_ptr + i * _chunk_size;
        _free_list.push((Node *)address);
    }
}

} // namespace noble_steed
#include "allocator.h"

namespace noble_steed
{
Mem_Resource_Base *get_default_resource()
{
    return std::pmr::get_default_resource();
}

Allocator::Allocator(const std::size_t total_size, Mem_Resource_Base *upstream)
    : _total_size(total_size), _used(0), _peak(0), _upstream(upstream), _start_ptr(_upstream->allocate(_total_size))
{
    _total_size = total_size;
    _used = 0;
}

Allocator::~Allocator()
{
    _upstream->deallocate(_start_ptr, _total_size);
}

void Allocator::reset()
{
    _used = 0;
    _peak = 0;
    do_reset();
}

Mem_Resource_Base *Allocator::upstream()
{
    return _upstream;
}

bool Allocator::do_is_equal(const Mem_Resource_Base &rhs) const noexcept
{
    return false;
}

} // namespace noble_steed
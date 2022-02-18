#include "mem_resource.h"

namespace noble_steed
{
Mem_Resource_Base *get_default_resource()
{
#ifdef PLATFORM_OSX
    return nullptr;//std::experimental::pmr::get_default_resource();
#else
    return std::pmr::get_default_resource();
#endif
}

Mem_Resource::Mem_Resource(const std::size_t total_size, Mem_Resource_Base *upstream)
    : _total_size(total_size), _used(0), _peak(0), _upstream(upstream), _start_ptr(_upstream->allocate(_total_size))
{
    _total_size = total_size;
    _used = 0;
}

Mem_Resource::~Mem_Resource()
{
    _upstream->deallocate(_start_ptr, _total_size);
}

void Mem_Resource::reset()
{
    _used = 0;
    _peak = 0;
    do_reset();
}

Mem_Resource_Base *Mem_Resource::upstream()
{
    return _upstream;
}

bool Mem_Resource::do_is_equal(const Mem_Resource_Base &rhs) const noexcept
{
    return false;
}

} // namespace noble_steed
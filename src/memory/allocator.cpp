#include "allocator.h"

namespace noble_steed
{

Allocator::Allocator(const std::size_t totalSize)
{
    _total_size = totalSize;
    _used = 0;
}

Allocator::~Allocator()
{
    _total_size = 0;
}

} // namespace noble_steed
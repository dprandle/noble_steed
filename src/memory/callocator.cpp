#include "callocator.h"
#include <stdlib.h> /* malloc, free */

namespace noble_steed
{

CAllocator::CAllocator() : Allocator(0)
{}

void CAllocator::init()
{}

CAllocator::~CAllocator()
{}

void *CAllocator::allocate(sizet size, sizet alignment)
{
    return malloc(size);
}

void CAllocator::free(void *ptr)
{
    ::free(ptr);
}
} // namespace noble_steed
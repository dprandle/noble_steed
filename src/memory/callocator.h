#pragma once

#include "allocator.h"

namespace noble_steed
{

class CAllocator : public Allocator
{
  public:
    CAllocator();

    virtual ~CAllocator();

    virtual void *allocate(sizet size, sizet alignment = 0) override;

    virtual void free(void *ptr) override;

    virtual void init() override;
};

} // namespace noble_steed
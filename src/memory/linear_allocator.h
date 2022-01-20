#pragma once

#include "allocator.h"

namespace noble_steed
{

class Linear_Allocator : public Allocator
{
  public:
    Linear_Allocator(sizet total_size);

    virtual ~Linear_Allocator();

    virtual void *allocate(sizet size, sizet alignment = 0) override;

    virtual void free(void *ptr) override;

    virtual void init() override;

    virtual void reset();

  protected:
    void *_start_ptr = nullptr;
    sizet _offset;
};

} // namespace noble_steed
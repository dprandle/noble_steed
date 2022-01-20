#pragma once

#include "allocator.h"

namespace noble_steed
{

class Stack_Allocator : public Allocator
{
  public:
    Stack_Allocator(sizet total_size);
    
    virtual ~Stack_Allocator();

    virtual void *allocate(sizet size, sizet alignment = 0) override;

    virtual void free(void *ptr) override;

    virtual void init() override;

    virtual void reset();

  protected:
    void *_start_ptr = nullptr;
    sizet _offset;

  private:
    struct Alloc_Header
    {
        char padding;
    };
};

} // namespace noble_steed
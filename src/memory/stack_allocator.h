#pragma once

#include "allocator.h"

namespace noble_steed::memory
{

class Stack_Allocator : public Allocator
{
    struct Alloc_Header
    {
        char padding;
    };

  public:
    Stack_Allocator(sizet total_size, Mem_Resource_Base * upstream);
    Stack_Allocator(sizet total_size);
    ~Stack_Allocator();

  private:
    void *do_allocate(sizet size, sizet alignment = 0) override;

    void do_deallocate(void *ptr, sizet bytes, sizet alignment=0) override;

    void do_reset() override;

    sizet _offset;
};

}
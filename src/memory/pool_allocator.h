#pragma once

#include "allocator.h"
#include "../container/stack_linked_list.h"

namespace noble_steed::memory
{
class Pool_Allocator : public Allocator
{
  public:
    Pool_Allocator(sizet total_size, sizet chunk_size);
    Pool_Allocator(sizet total_size, sizet chunk_size, Mem_Resource_Base *upstream);
    ~Pool_Allocator();

  private:
    void do_reset() override;

    void *do_allocate(sizet size, sizet alignment = 0) override;

    void do_deallocate(void *ptr, sizet bytes, sizet alignment = 0) override;
    
    struct Free_Header
    {};

    using Node = Stack_Linked_List<Free_Header>::Node;

    sizet _chunk_size;
    Stack_Linked_List<Free_Header> _free_list;
};
} // namespace noble_steed::memory
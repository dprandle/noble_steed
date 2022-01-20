#pragma once

#include "allocator.h"
#include "../container/stack_linked_list.h"

namespace noble_steed
{

class Pool_Allocator : public Allocator
{
  public:
    Pool_Allocator(sizet total_size, sizet chunk_size);

    virtual ~Pool_Allocator();

    virtual void *allocate(sizet size, sizet alignment = 0) override;

    virtual void free(void *ptr) override;

    virtual void init() override;

    virtual void reset();

  protected:
    struct Free_Header
    {};

    using Node = Stack_Linked_List<Free_Header>::Node;

    void *_start_ptr = nullptr;
    sizet _chunk_size;
    Stack_Linked_List<Free_Header> _free_list;
};

} // namespace noble_steed
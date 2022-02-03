#pragma once

#include "mem_resource.h"
#include "../container/linked_list.h"

namespace noble_steed
{
class Pool_Allocator : public Mem_Resource
{
    struct Free_Header
    {};

    using node = ll_node<Free_Header>;

  public:
    Pool_Allocator(sizet total_size, sizet chunk_size);
    Pool_Allocator(sizet total_size, sizet chunk_size, Mem_Resource_Base *upstream);
    ~Pool_Allocator();

  private:
    void do_reset() override;

    void *do_allocate(sizet size, sizet alignment = 0) override;

    void do_deallocate(void *ptr, sizet bytes, sizet alignment = 0) override;

    sizet _chunk_size;
    stack_linked_list<Free_Header> _free_list;
};
} // namespace noble_steed
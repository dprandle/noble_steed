#pragma once

#include "mem_resource.h"
#include "../container/linked_list.h"

namespace noble_steed
{

class free_list_mem_resource : public Mem_Resource
{
    struct Free_Header
    {
        sizet block_size;
    };

    struct Alloc_Header
    {
        sizet block_size;
        char padding;
    };

    using node = ll_node<Free_Header>;

  public:
    enum placement_policy
    {
        FIND_FIRST,
        FIND_BEST
    };

    free_list_mem_resource(sizet total_size, placement_policy p_policy, Mem_Resource_Base *upstream);
    free_list_mem_resource(sizet total_size, placement_policy p_policy);
    ~free_list_mem_resource();

  private:
    void do_reset() override;

    // TODO: Maybe this aligment should not default to 0?
    void *do_allocate(sizet size, sizet alignment = 0) override;

    void do_deallocate(void *ptr, sizet bytes, sizet alignment = 0) override;

    void coalescence(node *prev_block, node *free_block);

    void find(sizet size, sizet alignment, sizet &padding, node *&previous_node, node *&found_node);

    void find_best(sizet size, sizet alignment, sizet &padding, node *&previous_node, node *&found_node);

    void find_first(sizet size, sizet alignment, sizet &padding, node *&previous_node, node *&found_node);

    placement_policy _p_policy;
    singly_linked_list<Free_Header> _free_list;
};

} // namespace noble_steed
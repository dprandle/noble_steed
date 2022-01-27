#pragma once

#include "allocator.h"
#include "../container/singly_linked_list.h"

namespace noble_steed::memory
{

class Free_List_Allocator : public Allocator
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
    typedef Singly_Linked_List<Free_Header>::Node Node;

  public:
    enum Placement_Policy
    {
        FIND_FIRST,
        FIND_BEST
    };

    Free_List_Allocator(sizet total_size, Placement_Policy p_policy, Mem_Resource_Base *upstream);
    Free_List_Allocator(sizet total_size, Placement_Policy p_policy);
    ~Free_List_Allocator();

  private:
    void do_reset() override;

    // TODO: Maybe this aligment should not default to 0?
    void *do_allocate(sizet size, sizet alignment = 0) override;

    void do_deallocate(void *ptr, sizet bytes, sizet alignment = 0) override;

    void coalescence(Node *prev_block, Node *free_block);

    void find(sizet size, sizet alignment, sizet &padding, Node *&previous_node, Node *&found_node);

    void find_best(sizet size, sizet alignment, sizet &padding, Node *&previous_node, Node *&found_node);

    void find_first(sizet size, sizet alignment, sizet &padding, Node *&previous_node, Node *&found_node);

    Placement_Policy _p_policy;
    Singly_Linked_List<Free_Header> _free_list;
};

} // namespace noble_steed::memory
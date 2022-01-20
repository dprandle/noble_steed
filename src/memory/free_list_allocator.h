#pragma once

#include "allocator.h"
#include "../container/singly_linked_list.h"

namespace noble_steed
{

class Free_List_Allocator : public Allocator
{
  protected:
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

    Free_List_Allocator(sizet total_size, Placement_Policy p_policy);

    virtual ~Free_List_Allocator();

    virtual void *allocate(sizet size, sizet alignment = 0) override;

    virtual void free(void *ptr) override;

    virtual void init() override;

    virtual void reset();

  protected:
    void *_start_ptr = nullptr;
    Placement_Policy _p_policy;
    Singly_Linked_List<Free_Header> _free_list;

  private:
    void coalescence(Node *prev_block, Node *free_block);

    void find(sizet size, sizet alignment, sizet &padding, Node *&previous_node, Node *&found_node);

    void find_best(sizet size, sizet alignment, sizet &padding, Node *&previous_node, Node *&found_node);

    void find_first(sizet size, sizet alignment, sizet &padding, Node *&previous_node, Node *&found_node);

};

} // namespace noble_steed
#include <stdlib.h>
#include <cassert>
#include <limits>
#include <algorithm>

#include "free_list_allocator.h"
#include "utils.h"

namespace noble_steed
{

Free_List_Allocator::Free_List_Allocator(sizet totalSize, const Placement_Policy pPolicy) : Allocator(totalSize)
{
    _p_policy = pPolicy;
}

void Free_List_Allocator::init()
{
    if (_start_ptr != nullptr)
    {
        free(_start_ptr);
        _start_ptr = nullptr;
    }
    _start_ptr = malloc(_total_size);

    this->reset();
}

Free_List_Allocator::~Free_List_Allocator()
{
    free(_start_ptr);
    _start_ptr = nullptr;
}

void *Free_List_Allocator::allocate(sizet size, sizet alignment)
{
    sizet alloc_header_size = sizeof(Free_List_Allocator::Alloc_Header);
    assert("Allocation size must be bigger" && size >= sizeof(Node));
    assert("Alignment must be 8 at least" && alignment >= 8);

    // Search through the free list for a free block that has enough space to allocate our data
    sizet padding;
    Node *affected_node, *prev_node;
    this->find(size, alignment, padding, prev_node, affected_node);
    assert(affected_node != nullptr && "Not enough memory");

    sizet alignmentPadding = padding - alloc_header_size;
    sizet required_size = size + padding;

    sizet rest = affected_node->data.block_size - required_size;

    if (rest > 0)
    {
        // We have to split the block into the data block and a free block of size 'rest'
        Node *new_free_node = (Node *)((sizet)affected_node + required_size);
        new_free_node->data.block_size = rest;
        _free_list.insert(affected_node, new_free_node);
    }
    _free_list.remove(prev_node, affected_node);

    // Setup data block
    sizet header_addr = (sizet)affected_node + alignmentPadding;
    sizet data_addr = header_addr + alloc_header_size;
    ((Free_List_Allocator::Alloc_Header *)header_addr)->block_size = required_size;
    ((Free_List_Allocator::Alloc_Header *)header_addr)->padding = alignmentPadding;

    _used += required_size;
    _peak = std::max(_peak, _used);

    return (void *)data_addr;
}

void Free_List_Allocator::find(sizet size, sizet alignment, sizet &padding, Node *&prev_node, Node *&found_node)
{
    switch (_p_policy)
    {
    case FIND_FIRST:
        find_first(size, alignment, padding, prev_node, found_node);
        break;
    case FIND_BEST:
        find_best(size, alignment, padding, prev_node, found_node);
        break;
    }
}

void Free_List_Allocator::find_first(sizet size, sizet alignment, sizet &padding, Node *&prev_node, Node *&found_node)
{
    //Iterate list and return the first free block with a size >= than given size
    Node *it = _free_list.head, *it_prev = nullptr;

    while (it != nullptr)
    {
        padding = mem_util::calc_padding_with_header((sizet)it, alignment, sizeof(Free_List_Allocator::Alloc_Header));
        sizet required_space = size + padding;
        if (it->data.block_size >= required_space)
        {
            break;
        }
        it_prev = it;
        it = it->next;
    }
    prev_node = it_prev;
    found_node = it;
}

void Free_List_Allocator::find_best(sizet size, sizet alignment, sizet &padding, Node *&prev_node, Node *&found_node)
{
    // Iterate WHOLE list keeping a pointer to the best fit
    sizet smallest_diff = std::numeric_limits<sizet>::max();
    Node *bestBlock = nullptr;
    Node *it = _free_list.head, *it_prev = nullptr;
    while (it != nullptr)
    {
        padding = mem_util::calc_padding_with_header((sizet)it, alignment, sizeof(Free_List_Allocator::Alloc_Header));
        sizet required_space = size + padding;
        if (it->data.block_size >= required_space && (it->data.block_size - required_space < smallest_diff))
        {
            bestBlock = it;
        }
        it_prev = it;
        it = it->next;
    }
    prev_node = it_prev;
    found_node = bestBlock;
}

void Free_List_Allocator::free(void *ptr)
{
    // Insert it in a sorted position by the address number
    sizet current_addr = (sizet)ptr;
    sizet header_addr = current_addr - sizeof(Free_List_Allocator::Alloc_Header);
    const Free_List_Allocator::Alloc_Header *alloc_header{(Free_List_Allocator::Alloc_Header *)header_addr};

    Node *free_node = (Node *)(header_addr);
    free_node->data.block_size = alloc_header->block_size + alloc_header->padding;
    free_node->next = nullptr;

    Node *it = _free_list.head;
    Node *it_prev = nullptr;
    while (it != nullptr)
    {
        if (ptr < it)
        {
            _free_list.insert(it_prev, free_node);
            break;
        }
        it_prev = it;
        it = it->next;
    }

    _used -= free_node->data.block_size;

    // Merge contiguous nodes
    coalescence(it_prev, free_node);
}

void Free_List_Allocator::coalescence(Node *prev_node, Node *free_node)
{
    if (free_node->next != nullptr && (sizet)free_node + free_node->data.block_size == (sizet)free_node->next)
    {
        free_node->data.block_size += free_node->next->data.block_size;
        _free_list.remove(free_node, free_node->next);
    }

    if (prev_node != nullptr && (sizet)prev_node + prev_node->data.block_size == (sizet)free_node)
    {
        prev_node->data.block_size += free_node->data.block_size;
        _free_list.remove(prev_node, free_node);
    }
}

void Free_List_Allocator::reset()
{
    _used = 0;
    _peak = 0;
    Node *first_node = (Node *)_start_ptr;
    first_node->data.block_size = _total_size;
    first_node->next = nullptr;
    _free_list.head = nullptr;
    _free_list.insert(nullptr, first_node);
}

} // namespace noble_steed
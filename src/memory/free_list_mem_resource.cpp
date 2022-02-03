#include <stdlib.h>
#include <cassert>
#include <limits>
#include <algorithm>

#include "free_list_mem_resource.h"
#include "utils.h"

namespace noble_steed
{

free_list_mem_resource::free_list_mem_resource(sizet total_size, const placement_policy p_policy, Mem_Resource_Base *upstream)
    : Mem_Resource(total_size, upstream), _p_policy(p_policy), _free_list()
{
    do_reset();
}

free_list_mem_resource::free_list_mem_resource(sizet total_size, const placement_policy p_policy)
    : Mem_Resource(total_size, get_default_resource()), _p_policy(p_policy), _free_list()
{
    do_reset();
}

free_list_mem_resource::~free_list_mem_resource()
{}

void *free_list_mem_resource::do_allocate(sizet size, sizet alignment)
{
    sizet alloc_header_size = sizeof(free_list_mem_resource::Alloc_Header);
    assert("Allocation size must be bigger" && size >= sizeof(node));
    assert("Alignment must be 8 at least" && alignment >= 8);

    // Search through the free list for a free block that has enough space to allocate our data
    sizet padding;
    node *affected_node, *prev_node;
    this->find(size, alignment, padding, prev_node, affected_node);
    assert(affected_node != nullptr && "Not enough memory");

    sizet alignmentPadding = padding - alloc_header_size;
    sizet required_size = size + padding;

    sizet rest = affected_node->data.block_size - required_size;

    if (rest > 0)
    {
        // We have to split the block into the data block and a free block of size 'rest'
        node *new_free_node = (node *)((sizet)affected_node + required_size);
        new_free_node->data.block_size = rest;
        _free_list.insert(affected_node, new_free_node);
    }
    _free_list.remove(prev_node, affected_node);

    // Setup data block
    sizet header_addr = (sizet)affected_node + alignmentPadding;
    sizet data_addr = header_addr + alloc_header_size;
    ((free_list_mem_resource::Alloc_Header *)header_addr)->block_size = required_size;
    ((free_list_mem_resource::Alloc_Header *)header_addr)->padding = alignmentPadding;

    _used += required_size;
    _peak = std::max(_peak, _used);

    return (void *)data_addr;
}

void free_list_mem_resource::find(sizet size, sizet alignment, sizet &padding, node *&prev_node, node *&found_node)
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

void free_list_mem_resource::find_first(sizet size, sizet alignment, sizet &padding, node *&prev_node, node *&found_node)
{
    //Iterate list and return the first free block with a size >= than given size
    node *it = _free_list.head, *it_prev = nullptr;

    while (it != nullptr)
    {
        padding = util::calc_padding_with_header((sizet)it, alignment, sizeof(free_list_mem_resource::Alloc_Header));
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

void free_list_mem_resource::find_best(sizet size, sizet alignment, sizet &padding, node *&prev_node, node *&found_node)
{
    // Iterate WHOLE list keeping a pointer to the best fit
    sizet smallest_diff = std::numeric_limits<sizet>::max();
    node *bestBlock = nullptr;
    node *it = _free_list.head, *it_prev = nullptr;
    while (it != nullptr)
    {
        padding = util::calc_padding_with_header((sizet)it, alignment, sizeof(free_list_mem_resource::Alloc_Header));
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

void free_list_mem_resource::do_deallocate(void *ptr, sizet bytes, sizet)
{
    // Insert it in a sorted position by the address number
    sizet current_addr = (sizet)ptr;
    sizet header_addr = current_addr - sizeof(free_list_mem_resource::Alloc_Header);
    const free_list_mem_resource::Alloc_Header *alloc_header{(free_list_mem_resource::Alloc_Header *)header_addr};

    node *free_node = (node *)(header_addr);
    free_node->data.block_size = alloc_header->block_size + alloc_header->padding;

    assert(bytes == free_node->data.block_size && "Trying to free different amount of mem than stored in block header");

    free_node->next = nullptr;

    node *it = _free_list.head;
    node *it_prev = nullptr;
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

void free_list_mem_resource::coalescence(node *prev_node, node *free_node)
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

void free_list_mem_resource::do_reset()
{
    node *first_node = (node *)_start_ptr;
    first_node->data.block_size = _total_size;
    first_node->next = nullptr;
    _free_list.head = nullptr;
    _free_list.insert(nullptr, first_node);
}

} // namespace noble_steed
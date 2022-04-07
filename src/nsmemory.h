#pragma once

#include "basic_types.h"
#include "linked_list.h"

namespace noble_steed
{

enum mem_alloc_type
{
    MEM_ALLOC_FREE_LIST,
    MEM_ALLOC_POOL,
    MEM_ALLOC_STACK,
    MEM_ALLOC_LINEAR
};

struct free_header
{
    sizet block_size;
};

struct alloc_header
{
    sizet block_size;
    char padding;
};

struct stack_alloc_header
{
    char padding;
};

enum placement_policy
{
    FIND_FIRST,
    FIND_BEST
};

using mem_node = ll_node<free_header>;

struct mem_free_list
{
    placement_policy p_policy;
    singly_linked_list<free_header> free_list;
};

struct mem_pool
{
    sizet chunk_size;
    singly_linked_list<free_header> free_list;
};

struct mem_stack
{
    sizet offset;
};

struct mem_linear
{
    sizet offset;
};

struct mem_store
{
    /// Input parameter for alloc functions
    sizet total_size;

    /// Input parameter for which type of allocator we want
    mem_alloc_type alloc_type;

    sizet used;
    sizet peak;
    void *start;
    union
    {
        mem_free_list mfl;
        mem_pool mpool;
        mem_stack mstack;
        mem_linear mlin;
    };
};

void *ns_alloc(mem_store *mem, sizet bytes, sizet alignment=8);

template<class T>
T * ns_alloc(mem_store *mem, sizet alignment=8)
{
    return (T*)ns_alloc(mem, sizeof(T), alignment);
}

template<class T>
T * ns_new(mem_store *mem, sizet alignment=8)
{
    T * item = ns_alloc<T>(mem, alignment);
    new (item) T();
    return item;
}

void ns_free(mem_store *mem, void *item);

template<class T>
void ns_delete(mem_store *mem, T *item)
{
    item->~T();
    ns_free(mem, item);
}

/// Fill in mem->total_size and which type of allocator is needed, then call these functions
void mem_store_alloc(mem_store *mem);


/// Convenience function to set mem alloc_type, chunk size, and total_size to MEM_ALLOC_POOL, sizeof(T), and sizof(T)*item_count respectively
/// Calls mem_store_allo after setting those in mem_store. Well - duh - you can see the source its a template.
template<class T>
void mem_pool_alloc(mem_store *mem, sizet item_count)
{
    mem->alloc_type = MEM_ALLOC_POOL;
    mem->mpool.chunk_size = sizeof(T);
    mem->total_size = mem->mpool.chunk_size * item_count;
    mem_store_alloc(mem);
}

/// Convenience function to set mem alloc_type and total_size to MEM_ALLOC_FREE_LIST and total_byte_size respectively
/// Calls mem_store_allo after setting those in mem_store.
void mem_free_list_alloc(mem_store *mem, sizet total_byte_size);

void mem_store_free(mem_store *mem);
void mem_store_init(mem_store *mem);
void mem_store_term(mem_store *mem);

} // namespace noble_steed
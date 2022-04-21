#pragma once

#include <utility>

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

    /// If null, the allocator memory pool will be allocated with platform_alloc, otherwise the
    /// upstream_allocator's alloc will be used with ns_alloc (same is true for free). Once an allocator gets its memory,
    /// don't change this to something different as it will likely crash (cant free from an allocator different than allocated from)
    mem_store * upstream_allocator {nullptr};

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

template<class T, class... Args>
T * ns_new(mem_store *mem, sizet alignment, Args&&... args)
{
    T * item = ns_alloc<T>(mem, alignment);
    new (item) T(std::forward<Args>(args)...);
    return item;
}

template<class T, class... Args>
T * ns_new(mem_store *mem, Args&&... args)
{
    T * item = ns_alloc<T>(mem);
    new (item) T(std::forward<Args>(args)...);
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
/// Calls mem_store_alloc after setting those in mem_store.
void mem_free_list_alloc(mem_store *mem, sizet total_byte_size);

/// Convenience function to set mem alloc_type and total_size to MEM_ALLOC_STACK and total_byte_size respectively
/// Calls mem_store_alloc after setting those in mem_store.
void mem_stack_alloc(mem_store *mem, sizet total_byte_size);

void mem_store_free(mem_store *mem);
void mem_store_init(mem_store *mem);
void mem_store_term(mem_store *mem);

mem_store * global_allocator();
void set_global_allocator(mem_store * alloc);

} // namespace noble_steed
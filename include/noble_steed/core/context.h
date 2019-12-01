#pragma once

#include <FreeListAllocator.h>

#include <noble_steed/core/common.h>
#include <noble_steed/core/signal.h>
#include <noble_steed/container/hash_map.h>
#include <noble_steed/container/stack.h>

#include <glm/vec2.hpp>

#define ns_ctxt Context::inst()

class PoolAllocator;

namespace noble_steed
{
const String ENTITY_ALLOC_KEY = "Entity_Alloc";

// TODO: Create a way to register per-component values for this - some components will be added way more than others
// This is the pre-allocated amount for a component type
const uint16_t DEFAULT_COMP_ALLOC = 1000;

// TODO: This should also be a part of paremeters that can be set per-game basically
// This is the pre-allocated amount for entities - adjust this as required
const uint16_t DEFAULT_ENTITY_ALLOC = 1000;

// The min alloc size was determined by running in debugger and seing what the min size was
// required by the FreeListAllocator
const uint8_t MIN_ALLOC_SIZE = 16;

// The min align size was determined by running in debugger and seing what the min size was
// required by the FreeListAllocator
const uint8_t MIN_ALIGN_SIZE = 8;

// A Pool allocator must at least allocate a single chunk size of this size - ie even if a component is 5 bytes, each
// component will still be allocated 8 bytes
const uint8_t MIN_CHUNK_ALLOC_SIZE = 8;

class World;
class Logger;
class Component;
class Entity;
class Context
{
    SLOT_OBJECT
  public:
    Context();
    ~Context();

    void initialize(const Variant_Map & init_params = Variant_Map());

    void terminate();

    World * get_world();

    Logger * get_logger();

    template<class T>
    T * raw_malloc()
    {
        T * mem_ptr = static_cast<T *>(mem_free_list_.Allocate(sizeof(T)));
        return mem_ptr;
    }

    /// Get a block large enough to hold type T from our pre-allocation amount - should crash program if not enough
    /// memory. Args will be used to construct the item within the block.
    template<class T, class... Args>
    T * malloc(const Args &... args)
    {
        std::size_t type_size = sizeof(T);
        if (type_size < MIN_ALLOC_SIZE)
            type_size = MIN_ALLOC_SIZE;
        T * mem_ptr = static_cast<T *>(mem_free_list_.Allocate(type_size, MIN_ALIGN_SIZE));
        new (mem_ptr) T(args...);
        return mem_ptr;
    }

    template<class T>
    void free(T * to_free)
    {
        to_free->~T();
        raw_free(to_free);
    }

    void raw_free(void * to_free);

    template<class T, class... Args>
    T * create_component(const Args &... args)
    {
        rttr::type t = rttr::type::get<T>();
        T * comp_ptr = static_cast<T *>(create_component_(t));
        new (comp_ptr) T(args...);

        // Assign id to comp
        if (!id_stack_.empty())
        {
            comp_ptr->set_id(id_stack_.top());
            id_stack_.pop();
        }
        else
        {
            ++current_id_;
            comp_ptr->set_id(current_id_);
        }

        // Initialize
        comp_ptr->initialize();

        // If component id or onwer id changes need to handle that for all items that reference it
        sig_connect(comp_ptr->id_change, Context::on_component_id_change);
        sig_connect(comp_ptr->owner_id_change, Context::on_component_owner_id_change);

        return comp_ptr;
    }

    void destroy_component(Component * comp);

    static Context & inst();

  private:
    void on_component_id_change(glm::uvec2 id);

    void on_component_owner_id_change(glm::uvec2 owner_id);

    void create_entity_allocator_(const Hash_Map<String, rttr::variant> & init_params);

    void create_component_allocators_(const Hash_Map<String, rttr::variant> & init_params);

    void register_component_type_(const rttr::type & type, uint16_t expected_component_count);

    Component * create_component_(const rttr::type & type);

    PoolAllocator * get_comp_allocator_(const rttr::type & type);

    Hash_Map<rttr::type::type_id, PoolAllocator *> comp_allocators_;

    Vector<Component*> comp_ptrs_;

    PoolAllocator * ent_allocator_;

    Vector<Entity*> ent_ptrs;

    FreeListAllocator mem_free_list_;

    Stack<uint32_t> id_stack_;

    World * world_;

    Logger * logger_;

    static Context * s_this_;

    static uint32_t current_id_;
};
} // namespace noble_steed

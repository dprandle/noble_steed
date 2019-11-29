#pragma once

#include <FreeListAllocator.h>

#include <noble_steed/core/system.h>
#include <noble_steed/container/hash_map.h>
#include <noble_steed/container/stack.h>
#include <noble_steed/core/signal.h>

#include <glm/vec2.hpp>

#define ns_ctxt Context::inst()

class PoolAllocator;

namespace noble_steed
{
const uint16_t DEFAULT_COMP_ALLOC = 100;
const uint8_t MIN_ALLOC_SIZE = 16;
const uint8_t MIN_ALIGN_SIZE = 8;

class World;
class Logger;
class Component;

class Context
{
    SLOT_OBJECT
  public:
    Context();
    ~Context();

    void initialize();

    void terminate();

    World * get_world();

    Logger * get_logger();

    System sys_;

    template<class T>
    T * raw_malloc()
    {
        T * mem_ptr = static_cast<T *>(mem_free_list_.Allocate(sizeof(T)));
        return mem_ptr;
    }

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

    // template<class T>
    // PoolAllocator * get_comp_allocator()
    // {
    //     rttr::type t = rttr::type::get<T>();
    //     return get_comp_allocator_(t);
    // }

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

        sig_connect(comp_ptr->id_change, Context::on_component_id_change);
        sig_connect(comp_ptr->owner_id_change, Context::on_component_owner_id_change);

        return comp_ptr;
    }

    template<class T>
    void destroy_component(T * component)
    {
        component->terminate();

        // Take the id back
        id_stack_.push(component->get_id());

        // This should also call the signal to change the comp id
        component->set_id(0);

        sig_disconnect(component->id_change);
        sig_disconnect(component->owner_id_change);

        component->~T();
        rttr::type t = rttr::type::get<T>();
        destroy_component_(component, t);
    }

    static Context & inst();

  private:
    void on_component_id_change(glm::uvec2 id);

    void on_component_owner_id_change(glm::uvec2 owner_id);

    void register_component_type_(const rttr::type & type, uint16_t expected_component_count);

    Component * create_component_(const rttr::type & type);

    void destroy_component_(Component * comp, const rttr::type & type);

    PoolAllocator * get_comp_allocator_(const rttr::type & type);

    Hash_Map<rttr::type::type_id, PoolAllocator *> comp_allocators_;

    FreeListAllocator mem_free_list_;

    Stack<uint32_t> id_stack_;

    World * world_;

    Logger * logger_;

    static Context * s_this_;

    static uint32_t current_id_;
};
} // namespace noble_steed

#pragma once

#include <noble_steed/core/factory.h>
#include <noble_steed/core/signal.h>

#define ns_ctxt Context::inst()

namespace noble_steed
{

// A Pool allocator must at least allocate a single chunk size of this size - ie even if a component is 5 bytes, each
// component will still be allocated 8 bytes
const uint8_t MIN_CHUNK_ALLOC_SIZE = 8;

class Logger;
class Resource_Cache;
class World;
class System;
class Context
{
    SLOT_OBJECT
  public:
    Context();
    ~Context();

    void initialize(const Variant_Map & init_params = Variant_Map());

    void terminate();

    Logger * get_logger();

    World * get_world();

    Resource_Cache * get_resource_cache();

    template<class T>
    T * raw_malloc()
    {
        rttr::type type = rttr::type::get<T>();
        T * mem_ptr = static_cast<T *>(malloc_(type));
        return mem_ptr;
    }

    /// Get a block large enough to hold type T from our pre-allocation amount - should crash program if not enough
    // /// memory. Args will be used to construct the item within the block.
    template<class T, class... Args>
    T * malloc(const Args &... args)
    {
        rttr::type type = rttr::type::get<T>();
        T * mem_ptr = static_cast<T *>(malloc_(type));
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

    template<class T>
    void register_system_type(const Variant_Map & init_params=Variant_Map())
    {
        Factory * fac = nullptr;
        rttr::type t = rttr::type::get<T>();
        if (type_factories_.find(t.get_id()) != type_factories_.end())
            return;

        // No special allocator for systems
        fac = malloc<System_Factory_Type<T>>(&mem_free_list_);
        type_factories_[t.get_id()] = fac;
    }

    template<class T>
    void register_component_type(const Variant_Map & init_params=Variant_Map())
    {
        Factory * fac = nullptr;
        rttr::type t = rttr::type::get<T>();
        if (type_factories_.find(t.get_id()) != type_factories_.end())
            return;

        PoolAllocator * alloc = create_component_allocator_(t, init_params);
        fac = malloc<Component_Factory_Type<T>>(alloc);
        type_factories_[t.get_id()] = fac;
    }

    template<class T>
    void register_resource_type(const Variant_Map & init_params=Variant_Map())
    {
        Factory * fac = nullptr;
        rttr::type t = rttr::type::get<T>();
        if (type_factories_.find(t.get_id()) != type_factories_.end())
            return;

        // Create the resource allocator
        PoolAllocator * alloc = create_resource_allocator_(t, init_params);
        fac = malloc<Resource_Factory_Type<T>>(alloc);
        type_factories_[t.get_id()] = fac;
    }

    Factory * get_base_factory(const rttr::type & obj_type);

    template<class Base_Fac_Type>
    Base_Fac_Type * get_factory(const rttr::type & obj_type)
    {
        return static_cast<Base_Fac_Type *>(get_base_factory(obj_type));
    }

    static Context & inst();

    PoolAllocator * get_comp_allocator(const rttr::type & type);

    PoolAllocator * get_resource_allocator(const rttr::type & type);

    PoolAllocator * get_entity_allocator();

  private:
    void * malloc_(const rttr::type & type);

    PoolAllocator * create_entity_allocator_(const Hash_Map<String, rttr::variant> & init_params);

    PoolAllocator * create_component_allocator_(const rttr::type & type, const Variant_Map & init_params);

    PoolAllocator * create_resource_allocator_(const rttr::type & type, const Variant_Map & init_params);

    FreeListAllocator mem_free_list_;

    Hash_Map<u64, PoolAllocator *> comp_allocators_;

    Hash_Map<u64, PoolAllocator *> resource_allocators_;

    Hash_Map<u64, Factory *> type_factories_;

    PoolAllocator * ent_allocator_;

    Logger * logger_;

    World * world_;

    Resource_Cache * resource_cache_;

    static Context * s_this_;
};
} // namespace noble_steed

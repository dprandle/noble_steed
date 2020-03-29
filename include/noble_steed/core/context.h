#pragma once

#include <noble_steed/core/factory.h>
#include <noble_steed/core/signal.h>
#include <rttr/rttr_cast.h>

namespace noble_steed
{
#define ns_ctxt Context::inst()

#define str_hash(arg) ns_ctxt.hash_str(arg)

#define type_hash(arg) str_hash(arg.get_name().to_string())

// A Pool allocator must at least allocate a single chunk size of this size - ie even if a component is 5 bytes, each
// component will still be allocated 8 bytes
extern const uint8_t MIN_CHUNK_ALLOC_SIZE;

// Set the CWD to this absolute path on startup - at least try to
extern const String INIT_CWD_KEY;

class Logger;
class Resource_Cache;
class Resource;
class Component;
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

    template<class Derived>
    Pool_Factory<Derived> * register_pool_factory(const Variant_Map & init_params, u16 alloc_quantity)
    {
        rttr::type t = rttr::type::get<Derived>();
        u32 type_id = type_hash(t);
        if (contains_factory(type_id))
            return nullptr;

        PoolAllocator * alloc = create_pool_allocator_(t, alloc_quantity, init_params);
        if (alloc == nullptr)
            return nullptr;
        
        Pool_Factory<Derived> * fac = malloc<Pool_Factory<Derived>>(alloc);
        type_factories_[type_id] = fac;
        
        // Register converter func


        return fac;
    }

    template<class Derived>
    Free_List_Factory<Derived> * register_free_list_factory(FreeListAllocator * alloc)
    {
        rttr::type t = rttr::type::get<Derived>();
        u32 type_id = type_hash(t);
        if (contains_factory(type_id))
            return nullptr;

        Free_List_Factory<Derived> * fac = malloc<Free_List_Factory<Derived>>(alloc);
        type_factories_[type_id] = fac;
        return fac;
    }

    template<class T>
    Free_List_Factory<T> * register_system_type(const Variant_Map & init_params)
    {
        return register_free_list_factory<T>(&mem_free_list_);
    }

    template<class T>
    Pool_Factory<T> * register_component_type(const Variant_Map & init_params)
    {
        return register_pool_factory<T>(init_params, DEFAULT_COMP_ALLOC);
    }

    template<class T>
    Pool_Factory<T> * register_resource_type(const String & extension, const Variant_Map & init_params)
    {
        rttr::type t = rttr::type::get<T>();
        auto ret = register_pool_factory<T>(init_params, DEFAULT_RES_ALLOC);
        if (ret != nullptr)
            set_resource_extension_(t, extension);
        return ret;
    }

    template<class T>
    void set_resource_extension(const String & extension)
    {
        rttr::type t = rttr::type::get<T>();
        set_resource_extension_(t, extension);
    }

    template<class T>
    String get_resource_extension()
    {
        rttr::type t = rttr::type::get<T>();
        return get_resource_extension(t);
    }

    String get_resource_extension(const rttr::type & resource_type);

    u32 hash_str(const String & str);

    u32 get_extension_resource_type(const String & extension);

    Factory * get_factory(const rttr::type & obj_type);

    Factory * get_factory(u32 fac_id);

    template<class T>
    Factory * get_factory()
    {
        rttr::type t = rttr::type::get<T>();
        return get_factory(t);
    }

    bool contains_factory(u32 fac_id);

    bool contains_factory(const rttr::type & obj_type);

    static Context & inst();

    PoolAllocator * get_pool_allocator(const rttr::type & type);

    PoolAllocator * get_pool_allocator(u32 type_id);

  private:
    void * malloc_(const rttr::type & type);

    void register_default_types_(const Variant_Map & init_params);

    void set_resource_extension_(const rttr::type & resource_type, const String & extension);

    PoolAllocator * create_pool_allocator_(const rttr::type & type, u16 alloc_amount_for_type, const Variant_Map & init_params);

    bool destroy_comp_allocator_(const rttr::type & type);

    FreeListAllocator mem_free_list_;

    Hash_Map<u32, PoolAllocator *> pool_allocators_;

    // Key is hashed extension including the dot, and value is resource type id
    Hash_Map<u32, u32> extension_resource_type_;

    Hash_Map<u32, String> resource_type_extension_;

    Hash_Map<u32, Factory *> type_factories_;

    Logger * logger_;

    World * world_;

    Resource_Cache * resource_cache_;

    static Context * s_this_;
};
} // namespace noble_steed

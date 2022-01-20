#pragma once

#include "variant.h"
#include "signal.h"
#include "factory.h"
#include "../container/hash_set.h"

namespace noble_steed
{
#define ns_ctxt Context::inst()

#define str_hash(arg) ns_ctxt.hash_str(arg)

#define post_event(...) ns_ctxt.post_event_to_queues(__VA_ARGS__)

extern const u32 INVALID_ID;

// A Pool allocator must at least allocate a single chunk size of this size - ie even if a component is 5 bytes, each
// component will still be allocated 8 bytes
extern const uint8_t MIN_CHUNK_ALLOC_SIZE;

/// Set the CWD to this absolute path on startup - at least try to
extern const String INIT_CWD_KEY;

/// Command line tools used by engine location (directory)
extern const String COMMAND_LINE_TOOLS_DIR;

namespace init_param_key
{
namespace context
{
/// i8 - use the renderer or run without rendering at all
extern const String HEADLESS;
} // namespace context
} // namespace init_param_key

class Logger;
class Resource_Cache;
class Resource;
class Component;
class World;
class System;
class Context
{
  public:
    Context();
    ~Context();

    void initialize(const Variant_Map & init_params = Variant_Map());

    void terminate();

    Logger * get_logger();

    World * get_world();

    Resource_Cache * get_resource_cache();

    template<class T>
    T * raw_malloc(u32 element_count = 1)
    {
        type_index type = typeid(T);
        T * mem_ptr = static_cast<T *>(malloc_(type, sizeof(T), element_count));
        return mem_ptr;
    }

    /// Get a block large enough to hold type T from our pre-allocation amount - should crash program if not enough
    // /// memory. Args will be used to construct the item within the block.
    template<class T, class... Args>
    T * malloc(const Args &... args)
    {
        T * mem_ptr = raw_malloc<T>(1);
        new (mem_ptr) T(args...);
        return mem_ptr;
    }

    template<class T, class... Args>
    T * malloc_array(u32 array_size, const Args &... each_element_constructor_args)
    {
        T * mem_ptr = raw_malloc<T>(array_size);
        array_alloc_sizes[static_cast<void *>(mem_ptr)] = array_size;
        for (u32 i = 0; i < array_size; ++i)
            new (mem_ptr + i) T(each_element_constructor_args...);
        return mem_ptr;
    }

    template<class T>
    void free(T * to_free)
    {
        to_free->~T();
        raw_free(to_free);
    }

    template<class T>
    void free_array(T * to_free)
    {
        auto fiter = array_alloc_sizes.find(static_cast<void *>(to_free));
        if (fiter != array_alloc_sizes.end())
        {
            for (int i = 0; i < fiter->second; ++i)
                (to_free + i)->~T();
        }
        raw_free(to_free);
    }

    void raw_free(void * to_free);

    template<class Derived>
    Pool_Factory<Derived> * register_pool_factory(const Variant_Map & init_params, u16 alloc_quantity)
    {
        type_index type_ind = typeid(Derived);
        
        if (contains_factory(type_ind))
            return nullptr;

        Pool_Allocator * alloc = create_pool_allocator_(type_ind, sizeof(Derived), alloc_quantity, init_params);
        if (alloc == nullptr)
            return nullptr;

        Pool_Factory<Derived> * fac = malloc<Pool_Factory<Derived>>(alloc);
        type_factories_[type_ind] = fac;
        return fac;
    }

    template<class Derived>
    Free_List_Factory<Derived> * register_free_list_factory(Free_List_Allocator * alloc)
    {
        type_index type_ind = typeid(Derived);
        if (contains_factory(type_ind))
        {
            return nullptr;
        }

        Free_List_Factory<Derived> * fac = malloc<Free_List_Factory<Derived>>(alloc);
        type_factories_[type_ind] = fac;
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
        type_index t = typeid(T);
        if (!set_resource_extension_(t, extension))
            return nullptr;
        auto ret = register_pool_factory<T>(init_params, DEFAULT_RES_ALLOC);
        if (ret == nullptr)
            remove_resource_extension(t);
        return ret;
    }

    template<class T>
    bool set_resource_extension(const String & extension)
    {
        type_index t = typeid(T);
        return set_resource_extension_(t, extension);
    }

    template<class T>
    String get_resource_extension()
    {
        type_index t = typeid(T);
        return get_resource_extension(t);
    }

    String get_resource_extension(const type_index & resource_type);

    String_Hash hash_str(const String & str);

    type_index get_extension_resource_type(const String & extension);

    Factory * get_factory(const type_index & obj_type);

    template<class T>
    Factory * get_factory()
    {
        type_index t = typeid(T);
        return get_factory(t);
    }

    bool contains_factory(const type_index & obj_type);

    static Context & inst();

    Pool_Allocator * get_pool_allocator(const type_index & type);

    void subscribe_to_event(Context_Obj * obj, const String & event);

    void subscribe_to_event(Context_Obj * obj, u32 event_id);

    void unsubscribe_from_all(Context_Obj * obj);

    void unsubscribe_from_event(Context_Obj * obj, const String & event);

    void unsubscribe_from_event(Context_Obj * obj, u32 event_id);

    void post_event_to_queues(Event & event);

    void post_event_to_queues(const String & event_name, const Variant_Map & data = Variant_Map());

  private:
    void * malloc_(const type_index & type, sizet type_size, u32 elements);

    void register_default_types_(const Variant_Map & init_params);

    bool set_resource_extension_(const type_index & resource_type, const String & extension);

    bool remove_resource_extension(const type_index & resource_type);

    Pool_Allocator * create_pool_allocator_(const type_index & type, sizet size_of_type, u16 alloc_amount_for_type, const Variant_Map & init_params);

    bool destroy_comp_allocator_(const type_index & type);

    Free_List_Allocator mem_free_list_;

    Hash_Map<type_index, Pool_Allocator *> pool_allocators_;

    Hash_Map<void *, u32> array_alloc_sizes;

    // Key is hashed extension including the dot, and value is resource type id
    Hash_Map<String_Hash, type_index> extension_resource_type_;

    Hash_Map<type_index, String> resource_type_extension_;

    Hash_Map<type_index, Factory*> type_factories_;

    Logger * logger_;

    World * world_;

    Hash_Map<String_Hash, Hash_Set<Context_Obj *>> event_subscribers_;

    Resource_Cache * resource_cache_;

    static Context * s_this_;

    SLOT_OBJECT
};
} // namespace noble_steed

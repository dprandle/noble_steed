#include <noble_steed/core/context.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/io/filesystem.h>
#include <noble_steed/core/system.h>
#include <noble_steed/scene/entity.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/core/resource_cache.h>
#include <noble_steed/core/engine.h>
#include <noble_steed/io/input_translator.h>
#include <noble_steed/graphics/renderer.h>

// Defualt types to register
#include <noble_steed/scene/transform.h>
#include <noble_steed/scene/world_chunk.h>
#include <noble_steed/hash/crc32.h>
#include <noble_steed/io/input_map.h>
#include <noble_steed/graphics/shader.h>

namespace noble_steed
{
Context * Context::s_this_ = nullptr;
const uint8_t MIN_CHUNK_ALLOC_SIZE = 8;
const String INIT_CWD_KEY = "cwd";
const u32 INVALID_ID = static_cast<u32>(-1);
/// Command line tools used by engine location (directory)
const String COMMAND_LINE_TOOLS_DIR = "tools";

namespace init_param_key
{
namespace context
{
const String HEADLESS = "HEADLESS";
} // namespace context
} // namespace init_param_key

Context::Context()
    : mem_free_list_(100 * MB_SIZE, FreeListAllocator::FIND_FIRST),
      pool_allocators_(),
      array_alloc_sizes(),
      extension_resource_type_(),
      type_factories_(),
      logger_(nullptr),
      world_(nullptr),
      resource_cache_(nullptr)
{
    s_this_ = this;
    // Allocate memories
    mem_free_list_.Init();
}

Context::~Context()
{
    mem_free_list_.Reset();
}

void Context::initialize(const Variant_Map & init_params)
{
    auto fiter = init_params.find(INIT_CWD_KEY);
    if (fiter != init_params.end())
    {
        if (fiter->second.is_type<String>())
        {
            String val = fiter->second.get_value<String>();
            if (fs::exists(val))
            {
                fs::current_path(val);
            }
            else
            {
                wlog("Could not change cwd to {} as path does not exist", val);
            }
        }
        else
        {
            wlog("Passed in recognized key {} but value type was {} instead of string", INIT_CWD_KEY, String(fiter->second.get_type().get_name()));
        }
    }
    // Create log directory
    fs::create_directory("logs");

    // Initialize logger before other stuff so logging works
    logger_ = malloc<Logger>();
    logger_->initialize(init_params);
    resource_cache_ = malloc<Resource_Cache>();
    world_ = malloc<World>();

    // Now create the default context types...
    register_default_types_(init_params);

    // Do rest of initialization
    resource_cache_->initialize(init_params);
    world_->initialize(init_params);
}

void Context::terminate()
{
    world_->terminate();
    resource_cache_->terminate();

    free(world_);
    world_ = nullptr;

    free(resource_cache_);
    resource_cache_ = nullptr;

    // Free all factories
    while (type_factories_.begin() != type_factories_.end())
    {
        free(type_factories_.begin()->second);
        type_factories_.erase(type_factories_.begin());
    }

    // Kill logger last so that we can log stuff when needed
    logger_->terminate();
    free(logger_);
    logger_ = nullptr;

    // Free all pool allocators
    while (pool_allocators_.begin() != pool_allocators_.end())
    {
        free(pool_allocators_.begin()->second);
        pool_allocators_.erase(pool_allocators_.begin());
    }

    mem_free_list_.Reset();
}

Logger * Context::get_logger()
{
    return logger_;
}

World * Context::get_world()
{
    return world_;
}

Resource_Cache * Context::get_resource_cache()
{
    return resource_cache_;
}

Context & Context::inst()
{
    return *s_this_;
}

void Context::raw_free(void * to_free)
{
    mem_free_list_.Free(to_free);
}

u32 Context::get_extension_resource_type(const String & extension)
{
    u32 hash_id = str_hash(extension);
    auto fiter = extension_resource_type_.find(hash_id);
    if (fiter != extension_resource_type_.end())
        return fiter->second;
    return INVALID_ID;
}

void * Context::malloc_(const rttr::type & type, u32 elements)
{
    sizet type_size = type.get_sizeof() * elements;
    if (type_size < MIN_ALLOC_SIZE)
        type_size = MIN_ALLOC_SIZE;
    return mem_free_list_.Allocate(type_size, MIN_ALIGN_SIZE);
}
void Context::register_default_types_(const Variant_Map & init_params)
{
    using namespace init_param_key::context;

    // Systems
    register_system_type<Engine>(init_params);
    register_system_type<Input_Translator>(init_params);

    i8 headless = 0;
    grab_param(init_params, HEADLESS, headless);
    if (!headless)
        register_system_type<Renderer>(init_params);

    // Resources
    register_resource_type<World_Chunk>(".bbworld", init_params);
    register_resource_type<Input_Map>(".imap", init_params);
    register_resource_type<Shader>(".sc",init_params);

    // Entity
    register_pool_factory<Entity>(init_params, DEFAULT_ENTITY_ALLOC);

    // Components
    register_component_type<Transform>(init_params);
}

bool Context::set_resource_extension_(const rttr::type & resource_type, const String & extension)
{
    if (extension.empty() || extension.find_first_of('/') != String::npos || extension.find_first_of('\\') != String::npos)
    {
        wlog("Cannot register resource type because extension {} is invalid", extension);
        return false;
    }

    String actual_extension(extension);
    if (actual_extension[0] != '.')
        actual_extension = "." + actual_extension;

    u32 type_id = type_hash(resource_type);
    u32 ext_hash = str_hash(actual_extension);
    resource_type_extension_[type_id] = actual_extension;
    extension_resource_type_[ext_hash] = type_id;
    ilog("Setting extension for resource type {} and id {} to {} ({})", resource_type.get_name().to_string(), type_id, actual_extension, ext_hash);
    return true;
}

bool Context::remove_resource_extension(const rttr::type & resource_type)
{
    String extension = get_resource_extension(resource_type);
    if (extension.empty())
    {
        wlog("Could not remove extension for resource type {} as no extension entry was found", resource_type.get_name().to_string());
        return false;
    }

    u32 type_id = type_hash(resource_type);
    u32 ext_hash = str_hash(extension);
    u32 cnt = resource_type_extension_.erase(type_id);
    u32 cnt2 = extension_resource_type_.erase(ext_hash);
    assert(cnt == cnt2 && cnt == 1);
    ilog("Successfully removed resource extension entry {} for type {}", extension, resource_type.get_name().to_string());
    return true;
}

void Context::subscribe_to_event(Context_Obj * obj, const String & event)
{
    subscribe_to_event(obj, hash_str(event));
}

void Context::subscribe_to_event(Context_Obj * obj, u32 event_id)
{
    event_subscribers_[event_id].insert(obj);
    sig_connect(obj->destroyed, this, &Context::unsubscribe_from_all);
    ilog("Subscribed {} to event id {}", obj->get_derived_info().m_type.get_name().to_string(), event_id);
}

void Context::unsubscribe_from_all(Context_Obj * obj)
{
    auto iter = event_subscribers_.begin();
    while (iter != event_subscribers_.end())
    {
        size_t cnt = iter->second.erase(obj);
        if (cnt)
        {
            ilog("Unsubscribed {} from event id {}", obj->get_derived_info().m_type.get_name().to_string(), iter->first);
        }
        ++iter;
    }
}

void Context::unsubscribe_from_event(Context_Obj * obj, const String & event)
{
    unsubscribe_from_event(obj, hash_str(event));
}

void Context::unsubscribe_from_event(Context_Obj * obj, u32 event_id)
{
    auto fiter = event_subscribers_.find(event_id);
    if (fiter != event_subscribers_.end())
    {
        size_t cnt = fiter->second.erase(obj);
        if (cnt)
        {
            ilog("Unsubscribed {} from event id {}", obj->get_derived_info().m_type.get_name().to_string(), event_id);
        }
    }
}

void Context::post_event_to_queues(Event & event)
{
    event.timestamp = ns_eng->elapsed();
    auto iter = event_subscribers_.find(event.id);
    if (iter != event_subscribers_.end())
    {
        auto subs_iter = iter->second.begin();
        while (subs_iter != iter->second.end())
        {
            (*subs_iter)->push_event(event);
            ++subs_iter;
        }
    }
}

void Context::post_event_to_queues(const String & event_name, const Variant_Map & data)
{
    Event ev(event_name, data);
    post_event_to_queues(ev);
}

String Context::get_resource_extension(const rttr::type & resource_type)
{
    String ret;
    u32 type_id = type_hash(resource_type);
    auto fiter = resource_type_extension_.find(type_id);
    if (fiter != resource_type_extension_.end())
        ret = fiter->second;
    return ret;
}

u32 Context::hash_str(const String & str)
{
    return crc32(str.c_str(), str.size());
}

PoolAllocator * Context::create_pool_allocator_(const rttr::type & type, u16 alloc_amount_for_type, const Variant_Map & init_params)
{
    sizet type_size = type.get_sizeof();

    if (type_size < MIN_CHUNK_ALLOC_SIZE)
        type_size = MIN_CHUNK_ALLOC_SIZE;

    // Grab the overriding alloc amount if its there
    u16 alloc_amount = alloc_amount_for_type;
    String type_str(type.get_name());
    type_str += "_Alloc";
    auto fiter = init_params.find(type_str);
    if (fiter != init_params.end())
    {
        if (fiter->second.is_type<u16>())
            alloc_amount = fiter->second.get_value<u16>();
        else
            wlog("Passed in value for key {} but value was of incorrect type", type_str);
    }

    PoolAllocator * pool_alloc = ns_ctxt.malloc<PoolAllocator>(alloc_amount * type_size, type_size);

    pool_alloc->Init();
    u32 type_id = type_hash(type);
    pool_allocators_.emplace(type_id, pool_alloc);
    String str(type.get_name());
    ilog("Creating {0} byte pool allocator for type {1} - enough for {2} instances", alloc_amount * type_size, str, alloc_amount);
    return pool_alloc;
}

bool Context::destroy_comp_allocator_(const rttr::type & type)
{
    u32 type_id = type_hash(type);
    auto fiter = pool_allocators_.find(type_id);
    if (fiter != pool_allocators_.end())
    {
        free(fiter->second);
        pool_allocators_.erase(fiter);
        return true;
    }
    return false;
}

PoolAllocator * Context::get_pool_allocator(const rttr::type & type)
{
    u32 type_id = type_hash(type);
    return get_pool_allocator(type_id);
}

PoolAllocator * Context::get_pool_allocator(u32 type_id)
{
    auto fiter = pool_allocators_.find(type_id);
    if (fiter != pool_allocators_.end())
        return fiter->second;
    return nullptr;
}

Factory * Context::get_factory(const rttr::type & obj_type)
{
    u32 type_id = type_hash(obj_type);
    return get_factory(type_id);
}

Factory * Context::get_factory(u32 fac_id)
{
    auto fiter = type_factories_.find(fac_id);
    if (fiter != type_factories_.end())
        return fiter->second;
    return nullptr;
}

bool Context::contains_factory(u32 fac_id)
{
    return (get_factory(fac_id) != nullptr);
}

bool Context::contains_factory(const rttr::type & obj_type)
{
    return (get_factory(obj_type) != nullptr);
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Context>("Context").constructor<>();
    // .property("id_", &Component::initialize, registration::public_access)
    // .property("terminate", &Component::terminate, registration::public_access)
    // .property("owner_id", &Component::owner_id_, registration::private_access);
}

#include "variant.h"
#include "context.h"
#include "system.h"
#include "resource_cache.h"
#include "engine.h"

#include "../io/logger.h"
#include "../io/filesystem.h"
#include "../io/input_map.h"
#include "../io/input_translator.h"

#include "../scene/entity.h"
#include "../scene/world.h"
#include "../scene/transform.h"
#include "../scene/world_chunk.h"

#include "../graphics/renderer.h"
#include "../graphics/shader.h"
#include "../graphics/mesh.h"

// Defualt types to register
#include "../hash/crc32.h"

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
    : mem_free_list_(100 * MB_SIZE, Free_List_Allocator::FIND_FIRST),
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
    mem_free_list_.init();
}

Context::~Context()
{
    mem_free_list_.reset();
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
            //wlog("Passed in recognized key {} but value type was {} instead of string", INIT_CWD_KEY, String(fiter->second.get_type().get_name()));
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

    mem_free_list_.reset();
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
    mem_free_list_.free(to_free);
}

type_index Context::get_extension_resource_type(const String & extension)
{
    String_Hash hash_id = str_hash(extension);
    auto fiter = extension_resource_type_.find(hash_id);
    if (fiter != extension_resource_type_.end())
        return fiter->second;
    return INVALID_TYPE;
}

void * Context::malloc_(const type_index & type_ind, sizet type_size, u32 elements)
{
    if (type_size < MIN_ALLOC_SIZE)
        type_size = MIN_ALLOC_SIZE;
    return mem_free_list_.allocate(type_size, MIN_ALIGN_SIZE);
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
    register_resource_type<Mesh>(".msh", init_params);

    // Entity
    register_pool_factory<Entity>(init_params, DEFAULT_ENTITY_ALLOC);

    // Components
    register_component_type<Transform>(init_params);
}

bool Context::set_resource_extension_(const type_index & type_ind, const String & extension)
{
    if (extension.empty() || extension.find_first_of('/') != String::npos || extension.find_first_of('\\') != String::npos)
    {
        wlog("Cannot register resource type because extension {} is invalid", extension);
        return false;
    }

    String actual_extension(extension);
    if (actual_extension[0] != '.')
        actual_extension = "." + actual_extension;

    String_Hash ext_hash = str_hash(actual_extension);
    resource_type_extension_[type_ind] = actual_extension;
    //extension_resource_type_[ext_hash] = type_ind;
    extension_resource_type_.emplace(ext_hash, type_ind);
    ilog("Setting extension for resource type {} and id {} to {} ({})", type_ind.name(), type_ind.hash_code(), actual_extension, ext_hash);
    return true;
}

bool Context::remove_resource_extension(const type_index & type_ind)
{
    String extension = get_resource_extension(type_ind);
    if (extension.empty())
    {
        wlog("Could not remove extension for resource type {} as no extension entry was found", type_ind.name());
        return false;
    }

    u32 ext_hash = str_hash(extension);
    u32 cnt = resource_type_extension_.erase(type_ind);
    u32 cnt2 = extension_resource_type_.erase(ext_hash);
    assert(cnt == cnt2 && cnt == 1);
    ilog("Successfully removed resource extension entry {} for type {}", extension, type_ind.name());
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
    ilog("Subscribed {} to event id {}", typeid(*obj).name(), event_id);
}

void Context::unsubscribe_from_all(Context_Obj * obj)
{
    auto iter = event_subscribers_.begin();
    while (iter != event_subscribers_.end())
    {
        size_t cnt = iter->second.erase(obj);
        if (cnt)
        {
            ilog("Unsubscribed {} from event id {}", typeid(*obj).name(), iter->first);
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
            ilog("Unsubscribed {} from event id {}", typeid(*obj).name(), event_id);
        }
    }
}

void Context::post_event_to_queues(Event & event)
{
    //event.timestamp = ns_eng->elapsed();
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

String Context::get_resource_extension(const type_index & type_ind)
{
    String ret;
    auto fiter = resource_type_extension_.find(type_ind);
    if (fiter != resource_type_extension_.end())
        ret = fiter->second;
    return ret;
}

String_Hash Context::hash_str(const String & str)
{
    return crc32(str.c_str(), str.size());
}

Pool_Allocator * Context::create_pool_allocator_(const type_index & type_ind, sizet size_of_type, u16 alloc_amount_for_type, const Variant_Map & init_params)
{
    if (size_of_type < MIN_CHUNK_ALLOC_SIZE)
        size_of_type = MIN_CHUNK_ALLOC_SIZE;

    // Grab the overriding alloc amount if its there
    // TODO: Possibly add back in init param for alloc amount
    u16 alloc_amount = alloc_amount_for_type;
    // String type_str(type_ind.name());
    // type_str += "_Alloc";
    // auto fiter = init_params.find(type_str);
    // if (fiter != init_params.end())
    // {
    //     if (fiter->second.is_type<u16>())
    //         alloc_amount = fiter->second.get_value<u16>();
    //     else
    //         wlog("Passed in value for key {} but value was of incorrect type", type_str);
    // }

    Pool_Allocator * pool_alloc = ns_ctxt.malloc<Pool_Allocator>(alloc_amount * size_of_type, size_of_type);
    pool_alloc->init();
    pool_allocators_.emplace(type_ind, pool_alloc);
    ilog("Creating {0} byte pool allocator for type {1} - enough for {2} instances", alloc_amount * size_of_type, type_ind.name(), alloc_amount);
    return pool_alloc;
}

bool Context::destroy_comp_allocator_(const type_index & type_ind)
{
    auto fiter = pool_allocators_.find(type_ind);
    if (fiter != pool_allocators_.end())
    {
        free(fiter->second);
        pool_allocators_.erase(fiter);
        return true;
    }
    return false;
}

Pool_Allocator * Context::get_pool_allocator(const type_index & type_ind)
{
    auto fiter = pool_allocators_.find(type_ind);
    if (fiter != pool_allocators_.end())
        return fiter->second;
    return nullptr;
}

Factory * Context::get_factory(const type_index & type_ind)
{
    auto fiter = type_factories_.find(type_ind);
    if (fiter != type_factories_.end())
        return fiter->second;
    return nullptr;
}

bool Context::contains_factory(const type_index & type_ind)
{
    return (get_factory(type_ind) != nullptr);
}

} // namespace noble_steed
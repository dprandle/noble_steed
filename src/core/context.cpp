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
#include "../graphics/window.h"

namespace noble_steed
{

Context * Context::s_this_ = nullptr;
const uint8_t MIN_CHUNK_ALLOC_SIZE = 8;
const String INIT_CWD_KEY = "cwd";

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
    : _main_alloc(MB_SIZE * 4, free_list_mem_resource::placement_policy::FIND_FIRST),
      logger_(nullptr),
      world_(nullptr),
      resource_cache_(nullptr)
{
    s_this_ = this;
}

Context::~Context()
{}

bool Context::initialize(const SPtr<Window> & window, const Variant_Map & init_params)
{
    _window = window;
    if (!_window->initialize(init_params))
        return false;

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
    logger_ = static_cast<Logger*>(_main_alloc.allocate(sizeof(Logger)));
    logger_->initialize(init_params);
    resource_cache_ = static_cast<Resource_Cache*>(_main_alloc.allocate(sizeof(Resource_Cache)));
    world_ = static_cast<World*>(_main_alloc.allocate(sizeof(World)));

    // Do rest of initialization
    resource_cache_->initialize(init_params);
    world_->initialize(init_params);

    return true;
}

void Context::terminate()
{
    world_->terminate();
    resource_cache_->terminate();

    free(world_);
    world_ = nullptr;

    free(resource_cache_);
    resource_cache_ = nullptr;

    // Kill logger last so that we can log stuff when needed
    logger_->terminate();
    free(logger_);
    logger_ = nullptr;
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

void Context::subscribe_to_event(Context_Obj * obj, const String & event)
{
    subscribe_to_event(obj, Str_Hash(event));
}

void Context::subscribe_to_event(Context_Obj * obj, const Str_Hash & event_id)
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
    unsubscribe_from_event(obj, Str_Hash(event));
}

void Context::unsubscribe_from_event(Context_Obj * obj, const Str_Hash & event_id)
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

} // namespace noble_steed
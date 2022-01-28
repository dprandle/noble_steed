#pragma once

#include "variant.h"
#include "signal.h"

#include "../container/hash_set.h"

#include "../memory/free_list_allocator.h"

namespace noble_steed
{
#define ns_ctxt Context::inst()

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

class World;
class Logger;
class Resource_Cache;
class Resource;
class Component;
class Context_Obj;
struct Event;

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

    static Context & inst();

    void subscribe_to_event(Context_Obj * obj, const String & event);

    void subscribe_to_event(Context_Obj * obj, u32 event_id);

    void unsubscribe_from_all(Context_Obj * obj);

    void unsubscribe_from_event(Context_Obj * obj, const String & event);

    void unsubscribe_from_event(Context_Obj * obj, u32 event_id);

    void post_event_to_queues(Event & event);

    void post_event_to_queues(const String & event_name, const Variant_Map & data = Variant_Map());

  private:
    Free_List_Allocator _main_alloc;
    Logger * logger_;
    World * world_;
    Hash_Map<String_Hash, Hash_Set<Context_Obj *>> event_subscribers_;
    Resource_Cache * resource_cache_;
    static Context * s_this_;

    SLOT_OBJECT
};
} // namespace noble_steed

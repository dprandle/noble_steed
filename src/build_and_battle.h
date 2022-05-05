#pragma once
#include "math/vector2.h"
#include "buffer.h"
#include "nserror.h"
#include "nsstring.h"

namespace noble_steed
{
struct window;
struct mem_store;
inline constexpr const sizet EV_BUFFER_SIZE = 1024;

namespace event_type
{
enum
{
    WINDOW_RESIZE,
    INPUT_KEY_PRESS,
    INPUT_KEY_RELEASE
};
}

struct bbev_resize
{
    ivec2 old_size;
    ivec2 new_size;
};

struct bbev_input_key_press
{
    i16 key;
    i16 modifiers;
    vec2 normalized_mpos;
};

struct bbev_input_key_release
{
    i16 key;
    vec2 normalized_mpos;
};

struct bb_event
{
    int type;
    union
    {
        bbev_resize resize;
        bbev_input_key_press kpress;
        bbev_input_key_release krelease;
    };
};

small_str event_string(const bb_event &ev);

struct bb_state
{
    mem_store *mem;
    mem_store *frame_mem;
    bool running{false};
    array<bb_event, EV_BUFFER_SIZE> events;
};

void bb_init(bb_state *bstate);
void bb_update_and_render(bb_state *bstate);
void bb_term(bb_state *bstate);

} // namespace noble_steed
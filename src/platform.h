#pragma once

#include "nsmemory.h"
#include "build_and_battle.h"
#include "nsstring.h"
#include "math/vector2.h"

struct GLFWwindow;
namespace noble_steed
{

struct bb_state;
struct window;

enum struct window_type
{
    glfw
};

struct window_glfw
{
    GLFWwindow *hndl;
};

struct window
{
    window_type type;
    ivec2 current_size;
    vec2 norm_mpos;
    union
    {
        window_glfw wglfw;
    };
};

struct window_flags
{
    enum
    {
        VISIBLE = 1,          // Ignored for full screen windows
        INTIALLY_FOCUSED = 2, // Ignored for full screen and initially hidden windows
        DECORATED = 4,        // Ignored for full screen windows
        MAXIMIZE = 8,         // Ignored for full screen
        ALWAYS_ON_TOP = 16,   // Ignored for full screen
        FULLSCREEN = 32,
        FULLSCREEN_AUTO_ICONIFTY = 64,  // Ignored for non full screen windows
        FULLSCREEN_CENTER_CURSOR = 128, // Ignored for non full screen windows
    };
};

struct window_create_params
{
    i8 win_flags;
    ivec2 resolution;
    small_str title;
};

struct platform_ctxt
{
    window win;
    bb_state bstate;
    mem_store mem;
    mem_store frame_mem;
};

void platform_init(platform_ctxt * pform, window_create_params cp);
void platform_exec(platform_ctxt * pform);
void platform_term(platform_ctxt * pform);

}
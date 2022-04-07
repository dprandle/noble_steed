#pragma once

#include "nsstring.h"
#include "math/vector2.h"

struct GLFWwindow;
namespace noble_steed
{
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

window create_window(const window_create_params &param);

// void terminate();

// bool closed();

// void *get_native_window();

// void *get_native_display();

// GLFWwindow *get_glfw_window();

// itup2 get_size();

// itup2 get_framebuffer_size();

// dtup2 get_cursor_pos();

// dtup2 get_normalized_cursor_pos();
}
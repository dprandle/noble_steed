#pragma once

#include <noble_steed/core/common.h>

struct GLFWwindow;

namespace noble_steed
{
class Window
{
  public:
    Window();
    ~Window();

    bool initialize(const Variant_Map & init_params);

    void terminate();

    bool closed();

    void * get_native_window();

    GLFWwindow * get_glfw_window();

    itup2 get_size();

    dtup2 get_normalized_cursor_pos();

    dtup2 get_cursor_pos();

  private:
    GLFWwindow * window_;
    bool close_window_;

    static void glfw_resize_window_callback(GLFWwindow * window, i32 width, i32 height);
    static void glfw_focus_change_callback(GLFWwindow * window, i32 focused);
    static void glfw_close_window_callback(GLFWwindow * window);
    static void glfw_iconify_window_callback(GLFWwindow * window, i32 iconified);
    static void glfw_maximize_window_callback(GLFWwindow * window, i32 maximized);
    static void glfw_window_position_callback(GLFWwindow * window, i32 x_pos, i32 y_pos);
    static void glfw_franebuffer_resized_callback(GLFWwindow * window, i32 width, i32 height);
};

namespace init_param_key
{
namespace window
{
/// String - title of window - defaults to "New Window"
extern const String TITLE;

/// Bool - Should the window be fullscreen or not? If not specified will default to false
extern const String FULLSCREEN;

/// itup2 - Size of window in screen coordinates.. if not provided then for fullscreen will be default video mode size, for windowed
/// it will be half of that.
extern const String SIZE;

/// bool - Specify whether the windowed mode window will be visible on creation - ignored for fullscreen windows - default is true
extern const String VISIBLE;

/// bool - Specify whether the windowed mode window should have border and normal close/title bar/etc - ignored for full screen windows - default is true
extern const String DECORATED;

/// bool - Maximize window on first creation - ignored for full screen windows - defualt is false
extern const String MAXIMIZE;

/// bool - Always show this window on top of others - ignored for full screen windows - default is false
extern const String ALWAS_ON_TOP;

/// bool - Auto iconify fullscreen window when it looses focus - ignored for windowed mode windows - default is true
extern const String FULLSCREEN_AUTO_ICONIFY;

/// bool - Focus this window when first created - ignored for full screen and hidden windows - default is true
extern const String FOCUSED;

/// bool - Center the cursor on fullscreen windows - ignored for windowed mode windows - default is true
extern const String CENTER_CURSOR;

} // namespace window
} // namespace init_param_key

namespace events
{
namespace window_closed
{
extern const u32 id;
} // namespace window_closed

/// This event is posted whenever the window is resized
namespace window_resized
{
extern const u32 id;
/// itup2 - new screen size in screen coordinates
extern const String new_size;
} // namespace window_resized

/// This event is posted whenever the focus is gained or lost on the main window
namespace window_focus_change
{
extern const u32 id;
/// i8 - 1 for window focused and 0 for not
extern const String focused;
} // namespace window_focus_change

/// This event is posted whenever the window is iconified or restored from iconified. Iconified basically means minimized.
namespace window_iconified
{
extern const u32 id;
/// i8 - 1 for window iconified and 0 for not
extern const String iconified;
} // namespace window_iconified

/// This event is posted whenever the window is maximized or restored from maximized
namespace window_maximized
{
extern const u32 id;
/// i8 - 1 for window maximized and 0 for not
extern const String maximized;
} // namespace window_maximized

/// This event is posted whenever the window is moved - this event is never posted in full screen mode
namespace window_moved
{
extern const u32 id;
/// itup2 - new window position - upper left corner in screen coordinates
extern const String new_pos;
} // namespace window_moved

/// This event is posted whenever the framebuffer is resized (the main window default framebuffer)
namespace framebuffer_resized
{
extern const u32 id;
/// itup2 - new framebuffer size in pixels
extern const String new_size;
} // namespace framebuffer_resized

} // namespace events

} // namespace noble_steed
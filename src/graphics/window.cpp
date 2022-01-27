#include <bx/bx.h>
#include <bx/spscqueue.h>
#include <bx/thread.h>
#include <bx/debug.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "window.h"

#include "../core/variant.h"
#include "../core/context.h"
#include "../core/context_obj.h"
#include "../core/string_hash.h"

namespace noble_steed::graphics
{

Window *win = nullptr;

Window::Window() : window_(nullptr), close_window_(false)
{
    win = this;
}

Window::~Window()
{}

bool Window::initialize(const Variant_Map &init_params)
{
    using namespace init_param_key::window;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    itup2 default_size(mode->width, mode->height);

    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    // Apply to fullscreen windows
    bool auto_iconify = true;
    grab_param(init_params, FULLSCREEN_AUTO_ICONIFY, auto_iconify);
    glfwWindowHint(GLFW_AUTO_ICONIFY, (int)auto_iconify);

    bool center_cursor = true;
    grab_param(init_params, CENTER_CURSOR, center_cursor);
    glfwWindowHint(GLFW_CENTER_CURSOR, (int)center_cursor);

    // Apply to windowed windows
    bool win_visible = true;
    grab_param(init_params, VISIBLE, win_visible);
    glfwWindowHint(GLFW_VISIBLE, (int)win_visible);

    bool decorated = true;
    grab_param(init_params, DECORATED, decorated);
    glfwWindowHint(GLFW_DECORATED, (int)decorated);

    bool maximize = false;
    grab_param(init_params, MAXIMIZE, maximize);
    glfwWindowHint(GLFW_MAXIMIZED, (int)maximize);

    bool always_on_top = false;
    grab_param(init_params, ALWAS_ON_TOP, always_on_top);
    glfwWindowHint(GLFW_FLOATING, (int)always_on_top);

    // Window creation different for FS than windowed
    String window_title = "New Window";
    grab_param(init_params, TITLE, window_title);

    bool fullsreen = false;
    if (grab_param(init_params, FULLSCREEN, fullsreen))
    {
        itup2 window_size = default_size;
        grab_param(init_params, SIZE, window_size);
        window_ = glfwCreateWindow(window_size.x, window_size.y, window_title.c_str(), monitor, nullptr);
    }
    else
    {
        default_size.x /= 2;
        default_size.y /= 2;
        itup2 window_size = default_size;
        grab_param(init_params, SIZE, window_size);
        window_ = glfwCreateWindow(window_size.x, window_size.y, window_title.c_str(), nullptr, nullptr);
    }

    if (!window_)
    {
        return false;
    }
    glfwSetWindowSizeCallback(window_, glfw_resize_window_callback);
    glfwSetWindowCloseCallback(window_, glfw_close_window_callback);
    glfwSetWindowMaximizeCallback(window_, glfw_maximize_window_callback);
    glfwSetWindowIconifyCallback(window_, glfw_iconify_window_callback);
    glfwSetWindowPosCallback(window_, glfw_window_position_callback);
    glfwSetWindowFocusCallback(window_, glfw_focus_change_callback);
    glfwSetFramebufferSizeCallback(window_, glfw_franebuffer_resized_callback);
    glfwSetInputMode(window_, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    return true;
}

GLFWwindow *Window::get_glfw_window()
{
    return window_;
}

void *Window::get_native_display()
{
    return glfwGetX11Display();
}

void Window::terminate()
{
    glfwDestroyWindow(window_);
}

bool Window::closed()
{
    return close_window_;
}

itup2 Window::get_size()
{
    itup2 ret;
    glfwGetWindowSize(window_, &ret.w, &ret.h);
    return ret;
}

itup2 Window::get_framebuffer_size()
{
    itup2 ret;
    glfwGetFramebufferSize(window_, &ret.w, &ret.h);
    return ret;
}

dtup2 Window::get_normalized_cursor_pos()
{
    dtup2 ret = get_cursor_pos();
    itup2 scrn = get_size();
    ret.w /= scrn.w;
    ret.h /= scrn.h;
    return ret;
}

dtup2 Window::get_cursor_pos()
{
    dtup2 ret;
    glfwGetCursorPos(window_, &ret.w, &ret.h);
    return ret;
}

void *Window::get_native_window()
{
#ifdef PLATFORM_OSX
    return glfwGetCocoaWindow(window_);
#elif PLATFORM_LINUX
    return reinterpret_cast<void *>(glfwGetX11Window(window_));
#endif
    return nullptr;
}

void Window::glfw_resize_window_callback(GLFWwindow *window, i32 width, i32 height)
{
    using namespace events;
    Event ev;
    ev.id = window::resized::id;
    ev.data[window::resized::new_size] = itup2(width, height);
    post_event(ev);
}

void Window::glfw_focus_change_callback(GLFWwindow *window, i32 focused)
{
    using namespace events;
    Event ev;
    ev.id = window::focus_change::id;
    ev.data[window::focus_change::focused] = i8(focused);
    post_event(ev);
}

void Window::glfw_close_window_callback(GLFWwindow *window)
{
    using namespace events;
    Event ev;
    ev.id = window::closed::id;
    post_event(ev);
}

void Window::glfw_iconify_window_callback(GLFWwindow *window, i32 iconified)
{
    using namespace events;
    Event ev;
    ev.id = window::iconified::id;
    ev.data[window::iconified::iconified] = i8(iconified);
    post_event(ev);
}

void Window::glfw_maximize_window_callback(GLFWwindow *window, i32 maximized)
{
    using namespace events;
    Event ev;
    ev.id = window::maximized::id;
    ev.data[window::maximized::maximized] = i8(maximized);
    post_event(ev);
}

void Window::glfw_window_position_callback(GLFWwindow *window, i32 x_pos, i32 y_pos)
{
    using namespace events;
    Event ev;
    ev.id = window::moved::id;
    ev.data[window::moved::new_pos] = itup2(x_pos, y_pos);
    post_event(ev);
}

void Window::glfw_franebuffer_resized_callback(GLFWwindow *window, i32 width, i32 height)
{
    using namespace events;
    Event ev;
    ev.id = window::framebuffer_resized::id;
    ev.data[window::framebuffer_resized::new_size] = itup2(width, height);
    post_event(ev);
}

namespace init_param_key::window
{
/// String - title of window - defaults to "New Window"
const String TITLE = "TITLE";

/// Bool - Should the window be fullscreen or not? If not specified will default to false
const String FULLSCREEN = "FULLSCREEN";

/// itup2 - Size of window in screen coordinates.. if not provided then for fullscreen will be default video mode size, for windowed
/// it will be half of that.
const String SIZE = "SIZE";

/// bool - Specify whether the windowed mode window will be visible on creation - ignored for fullscreen windows - default is true
const String VISIBLE = "VISIBLE";

/// bool - Specify whether the windowed mode window should have border and normal close/title bar/etc - ignored for full screen windows - default is true
const String DECORATED = "DECORATED";

/// bool - Maximize window on first creation - ignored for full screen windows - defualt is false
const String MAXIMIZE = "MAXIMIZE";

/// bool - Always show this window on top of others - ignored for full screen windows - default is false
const String ALWAS_ON_TOP = "ALWAS_ON_TOP";

/// bool - Auto iconify fullscreen window when it looses focus - ignored for windowed mode windows - default is true
const String FULLSCREEN_AUTO_ICONIFY = "FULLSCREEN_AUTO_ICONIFY";

/// bool - Focus this window when first created - ignored for full screen and hidden windows - default is true
const String FOCUSED = "FOCUSED";

/// bool - Center the cursor on fullscreen windows - ignored for windowed mode windows - default is true
const String CENTER_CURSOR = "CENTER_CURSOR";

} // namespace init_param_key::window

namespace events
{
namespace window::closed
{
const u32 id = Str_Hash("Window_Closed").value();
} // namespace window::closed

namespace window::resized
{
const u32 id = Str_Hash("Window_Resized").value();
const String new_size = "new_size";
} // namespace window::resized

namespace window::focus_change
{
const u32 id = Str_Hash("Window_Focus_Change").value();
const String focused = "focused";
} // namespace window::focus_change

namespace window::iconified
{
const u32 id = Str_Hash("Window_Iconified").value();
const String iconified = "iconified";
} // namespace window::iconified

namespace window::maximized
{
const u32 id = Str_Hash("Window_Maximized").value();
const String maximized = "maximized";
} // namespace window::maximized

namespace window::moved
{
const u32 id = Str_Hash("Window_Moved").value();
const String new_pos = "new_pos";
} // namespace window::moved

namespace window::framebuffer_resized
{
const u32 id = Str_Hash("Framebuffer_Resized").value();
const String new_size = "new_size";
} // namespace window::framebuffer_resized
} // namespace events
} // namespace noble_steed::graphics
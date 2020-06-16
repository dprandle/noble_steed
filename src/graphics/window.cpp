#include <noble_steed/graphics/window.h>
#include <noble_steed/core/context.h>

#include <bx/bx.h>
#include <bx/spscqueue.h>
#include <bx/thread.h>
#include <bx/debug.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace noble_steed
{
Window * win = nullptr;

Window::Window() : window_(nullptr), close_window_(false)
{
    win = this;
}

Window::~Window()
{}

bool Window::initialize(const Variant_Map & init_params)
{
    using namespace Init_Params::Window;

    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * mode = glfwGetVideoMode(monitor);
    itup2 default_size(mode->width, mode->height);

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
    glfwSetCursorPosCallback(window_, glfw_cursor_pos_callback);
    glfwSetWindowPosCallback(window_, glfw_window_position_callback);
    glfwSetWindowFocusCallback(window_, glfw_focus_change_callback);
    glfwSetFramebufferSizeCallback(window_, glfw_franebuffer_resized_callback);
    glfwSetInputMode(window_, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    return true;
}

GLFWwindow * Window::get_glfw_window()
{
    return window_;
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

void * Window::get_native_window()
{
#ifdef PLATFORM_OSX
    return glfwGetCocoaWindow(window_);
#elif PLATFORM_LINUX
    return reinterpret_cast<void *>(glfwGetX11Window(window_));
#endif
    return nullptr;
}

void Window::glfw_cursor_pos_callback(GLFWwindow * window_, double x_pos, double y_pos)
{
    using namespace Events;
    Event ev;
    ev.id = Cursor_Moved::id;
    ev.data[Cursor_Moved::new_pos] = dtup2(x_pos, y_pos);
    post_event(ev);
}

void Window::glfw_resize_window_callback(GLFWwindow * window, i32 width, i32 height)
{
    using namespace Events;
    Event ev;
    ev.id = Window_Resized::id;
    ev.data[Window_Resized::new_size] = itup2(width, height);
    post_event(ev);
}

void Window::glfw_focus_change_callback(GLFWwindow * window, i32 focused)
{
    using namespace Events;
    Event ev;
    ev.id = Window_Focus_Change::id;
    ev.data[Window_Focus_Change::focused] = focused;
    post_event(ev);
}

void Window::glfw_close_window_callback(GLFWwindow * window)
{
    using namespace Events;
    Event ev;
    ev.id = Window_Closed::id;
    post_event(ev);
}

void Window::glfw_iconify_window_callback(GLFWwindow * window, i32 iconified)
{
    using namespace Events;
    Event ev;
    ev.id = Window_Iconified::id;
    ev.data[Window_Iconified::iconified] = iconified;
    post_event(ev);
}

void Window::glfw_maximize_window_callback(GLFWwindow * window, i32 maximized)
{
    using namespace Events;
    Event ev;
    ev.id = Window_Maximized::id;
    ev.data[Window_Maximized::maximized] = maximized;
    post_event(ev);
}

void Window::glfw_window_position_callback(GLFWwindow * window, i32 x_pos, i32 y_pos)
{
    using namespace Events;
    Event ev;
    ev.id = Window_Moved::id;
    ev.data[Window_Moved::new_pos] = itup2(x_pos, y_pos);
    post_event(ev);
}

void Window::glfw_franebuffer_resized_callback(GLFWwindow * window, i32 width, i32 height)
{
    using namespace Events;
    Event ev;
    ev.id = Framebuffer_Resized::id;
    ev.data[Framebuffer_Resized::new_size] = itup2(width, height);
    post_event(ev);
}

namespace Init_Params
{
namespace Window
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

} // namespace Window
} // namespace Init_Params


namespace Events
{
namespace Window_Closed
{
const u32 id = str_hash("Window_Closed");
} // namespace Window_Closed

namespace Cursor_Moved
{
const u32 id = str_hash("Cursor_Moved");
const String new_pos = "new_pos"; // dtup2
} // namespace Cursor_Moved

namespace Window_Resized
{
const u32 id = str_hash("Window_Resized");
const String new_size = "new_size"; // itup2
} // namespace Window_Resized

namespace Window_Focus_Change
{
const u32 id = str_hash("Window_Focus_Change");
const String focused = "focused"; // i32
} // namespace Window_Focus_Change

namespace Window_Iconified
{
const u32 id = str_hash("Window_Iconified");
const String iconified = "iconified"; // i32
} // namespace Window_Iconified

namespace Window_Maximized
{
const u32 id = str_hash("Window_Maximized");
const String maximized = "maximized"; // i32
} // namespace Window_Maximized

namespace Window_Moved
{
const u32 id = str_hash("Window_Moved");
const String new_pos = "new_pos"; // itup2
} // namespace Window_Moved

namespace Framebuffer_Resized
{
const u32 id = str_hash("Framebuffer_Resized");
const String new_size = "new_size"; // itup2
} // namespace Framebuffer_Resized

} // namespace Events
} // namespace noble_steed
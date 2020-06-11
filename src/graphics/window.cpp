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
    window_ = glfwCreateWindow(1024, 768, "TBD", nullptr, nullptr);
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
    glfwSetFramebufferSizeCallback(window_,glfw_franebuffer_resized_callback);
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

namespace Events
{
namespace Window_Closed
{
const int id = str_hash("Window_Closed");
} // namespace Window_Closed

namespace Cursor_Moved
{
const int id = str_hash("Cursor_Moved");
const String new_pos = "new_pos"; // dtup2
} // namespace Cursor_Moved

namespace Window_Resized
{
const int id = str_hash("Window_Resized");
const String new_size = "new_size"; // itup2
} // namespace Window_Resized

namespace Window_Focus_Change
{
const int id = str_hash("Window_Focus_Change");
const String focused = "focused"; // i32
} // namespace Window_Focus_Change

namespace Window_Iconified
{
const int id = str_hash("Window_Iconified");
const String iconified = "iconified"; // i32
} // namespace Window_Iconified

namespace Window_Maximized
{
const int id = str_hash("Window_Maximized");
const String maximized = "maximized"; // i32
} // namespace Window_Maximized

namespace Window_Moved
{
const int id = str_hash("Window_Moved");
const String new_pos = "new_pos"; // itup2
} // namespace Window_Moved

namespace Framebuffer_Resized
{
const int id = str_hash("Framebuffer_Resized");
const String new_size = "new_size"; // itup2
} // namespace Window_Resized

} // namespace Events
} // namespace noble_steed
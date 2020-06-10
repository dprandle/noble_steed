#include <noble_steed/graphics/window.h>

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
    glfwSetWindowMaximizeCallback(window_,glfw_maximize_window_callback);
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

void * Window::get_native_window()
{
#ifdef PLATFORM_OSX
    return glfwGetCocoaWindow(window_);
#endif
    return nullptr;
}

void Window::glfw_cursor_pos_callback(GLFWwindow * window_, double x_pos, double y_pos)
{}

void Window::glfw_resize_window_callback(GLFWwindow * window, i32 width, i32 height)
{
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, width, height);
    bgfx::touch(0);
}

void Window::glfw_focus_change_callback(GLFWwindow * window, int give_or_taken)
{}

void Window::glfw_close_window_callback(GLFWwindow * window)
{
    win->close_window_ = true;
}

void Window::glfw_minimize_window_callback(GLFWwindow * window, int min_or_restore)
{}

void Window::glfw_maximize_window_callback(GLFWwindow * window, int max_or_restore)
{}

void Window::glfw_window_position_callback(GLFWwindow * window, int x_pos, int y_pos)
{}
}
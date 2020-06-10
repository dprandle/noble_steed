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

  private:
    GLFWwindow * window_;
    bool close_window_;

    static void glfw_cursor_pos_callback(GLFWwindow * window_, double x_pos, double y_pos);
    static void glfw_resize_window_callback(GLFWwindow* window, i32 width, i32 height);
    static void glfw_focus_change_callback(GLFWwindow* window, int give_or_taken);
    static void glfw_close_window_callback(GLFWwindow* window);
    static void glfw_minimize_window_callback(GLFWwindow * window, int min_or_restore);
    static void glfw_maximize_window_callback(GLFWwindow * window, int max_or_restore);
    static void glfw_window_position_callback(GLFWwindow * window, int x_pos, int y_pos);

};
} // namespace noble_steed
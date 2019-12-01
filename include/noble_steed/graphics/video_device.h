#pragma once
#include <noble_steed/core/common.h>

int GLFW_func(void);
struct GLFWwindow;

namespace noble_steed
{

class Video_Device
{
    public:
    
    Video_Device();
    ~Video_Device();

    void init();

    GLFWwindow * window_;

    private:

    
    static void glfw_key_press_callback(GLFWwindow * window_, i32 pKey, i32 scancode_, i32 action_, i32 mods_);
    static void glfw_mouse_button_callback(GLFWwindow * window_, i32 pButton, i32 action_, i32 mods_);
    static void glfw_cursor_pos_callback(GLFWwindow * window_, double x_pos, double y_pos);
    static void glfw_scroll_callback(GLFWwindow * window_, double x_offset, double y_offset);
    static void glfw_resize_window_callback(GLFWwindow* window, i32 width, i32 height);
    static void glfw_focus_change_callback(GLFWwindow* window, int give_or_taken);
    static void glfw_close_window_callback(GLFWwindow* window);
    static void glfw_minimize_window_callback(GLFWwindow * window, int min_or_restore);
    static void glfw_maximize_window_callback(GLFWwindow * window, int max_or_restore);
    static void glfw_window_position_callback(GLFWwindow * window, int x_pos, int y_pos);
    static void glfw_error_callback(i32 error, const char * description);
    
};

}
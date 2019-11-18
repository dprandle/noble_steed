#pragma once
#include <cstdint>

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

    
    static void glfw_key_press_callback(GLFWwindow * window_, int32_t pKey, int32_t scancode_, int32_t action_, int32_t mods_);
    static void glfw_mouse_button_callback(GLFWwindow * window_, int32_t pButton, int32_t action_, int32_t mods_);
    static void glfw_cursor_pos_callback(GLFWwindow * window_, double x_pos, double y_pos);
    static void glfw_scroll_callback(GLFWwindow * window_, double x_offset, double y_offset);
    static void glfw_resize_window_callback(GLFWwindow* window, int32_t width, int32_t height);
    static void glfw_focus_change_callback(GLFWwindow* window, int give_or_taken);
    static void glfw_close_window_callback(GLFWwindow* window);
    static void glfw_minimize_window_callback(GLFWwindow * window, int min_or_restore);
    static void glfw_maximize_window_callback(GLFWwindow * window, int max_or_restore);
    static void glfw_window_position_callback(GLFWwindow * window, int x_pos, int y_pos);
    static void glfw_error_callback(int32_t error, const char * description);
    
};

}
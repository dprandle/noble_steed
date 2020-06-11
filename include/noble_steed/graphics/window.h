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

    static void glfw_cursor_pos_callback(GLFWwindow * window_, double x_pos, double y_pos);
    static void glfw_resize_window_callback(GLFWwindow * window, i32 width, i32 height);
    static void glfw_focus_change_callback(GLFWwindow * window, i32 focused);
    static void glfw_close_window_callback(GLFWwindow * window);
    static void glfw_iconify_window_callback(GLFWwindow * window, i32 iconified);
    static void glfw_maximize_window_callback(GLFWwindow * window, i32 maximized);
    static void glfw_window_position_callback(GLFWwindow * window, i32 x_pos, i32 y_pos);
    static void glfw_franebuffer_resized_callback(GLFWwindow * window, i32 width, i32 height);
};

namespace Events
{
namespace Window_Closed
{
extern const int id;
} // namespace Window_Closed

namespace Cursor_Moved
{
extern const int id;
extern const String new_pos; // dtup2
} // namespace Cursor_Moved

namespace Window_Resized
{
extern const int id;
extern const String new_size; // itup2
} // namespace Window_Resized

namespace Window_Focus_Change
{
extern const int id;
extern const String focused; // i32
} // namespace Window_Focus_Change

namespace Window_Iconified
{
extern const int id;
extern const String iconified; // i32
} // namespace Window_Iconified

namespace Window_Maximized
{
extern const int id;
extern const String maximized; // i32
} // namespace Window_Maximized

namespace Window_Moved
{
extern const int id;
extern const String new_pos; // itup2
} // namespace Window_Moved

namespace Framebuffer_Resized
{
extern const int id;
extern const String new_size; // itup2
} // namespace Window_Resized


} // namespace Events

} // namespace noble_steed
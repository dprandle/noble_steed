#pragma once

#include "window.h"

struct GLFWwindow;

namespace noble_steed
{

class GLFW_Window : public Window
{
  public:
    GLFW_Window();
    ~GLFW_Window();

    bool initialize(const Variant_Map &init_params);

    void terminate();

    bool closed();

    void *get_native_window();

    void *get_native_display();

    GLFWwindow *get_glfw_window();

    itup2 get_size();

    itup2 get_framebuffer_size();

    dtup2 get_cursor_pos();

    dtup2 get_normalized_cursor_pos();

  private:
    GLFWwindow *window_;
    bool close_window_;
};

} // namespace noble_steed
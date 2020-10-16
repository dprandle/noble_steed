#pragma once

#include "common.h"
#include "variant.h"

namespace noble_steed
{
class Context;
class Window;

class Application
{
  public:
    Application();
    ~Application();

    void initialize(const Variant_Hash & init_params = Variant_Hash());
    void terminate();

    Window * get_window();

    Context * get_context();

    static Application & inst();

    int exec();

  private:

    Window * window_;
    Context * ctxt_;

    static Application * this_global_ptr;

    static void glfw_error_callback(i32 error, const char * description);
};

#define app Application::inst()

} // namespace noble_steed
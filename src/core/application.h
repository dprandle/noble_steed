#pragma once

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

    Context *get_context();

    static Application &inst();

    int exec(const SPtr<Window> &window, const Variant_Map &init_params);

  private:
    UPtr<Context> ctxt_;
    static Application *this_global_ptr;
};

#define application Application::inst()

} // namespace noble_steed
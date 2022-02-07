#include <stdio.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <iostream>
#include <sstream>

#include "application.h"
#include "variant.h"
#include "context.h"
#include "engine.h"
#include "../io/logger.h"
#include "../scene/world.h"
#include "../graphics/window.h"

#include <pybind11/embed.h>

namespace noble_steed
{
Application * Application::this_global_ptr = nullptr;

Application::Application() : ctxt_(make_unique<Context>())
{
    this_global_ptr = this;
}

Application::~Application()
{}

Application & Application::inst()
{
    return *this_global_ptr;
}

int Application::exec(const SPtr<Window> & window, const Variant_Map & init_params)
{
    if (!ctxt_->initialize(window, init_params))
        return -1;
    
    // pybind11::scoped_interpreter guard{};
    // auto m = pybind11::module::import("Noble_Steed");
    // pybind11::object obj = pybind11::cast(&Context::inst());
    // m.attr("context") = obj;
    // dlog("Scooby");
    // Engine * eng = ns_eng;
    // eng->set_running(true);
    
    
    // while (eng->is_running())
    // {
    //     glfwPollEvents();
    //     eng->run_frame();
    // }

    ctxt_->terminate();
    return 0;

    // pybind11::exec(R"(
    //     import Noble_Steed
    //     Noble_Steed.context.sys.log_internal()
    //     Noble_Steed.context.sys.internal = "poopy yo yo"
    // )");
    // ctxt.sys_.log_internal();
}

Context * Application::get_context()
{
    return ctxt_.get();
}

} // namespace noble_steed
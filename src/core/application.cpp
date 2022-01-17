#include <stdio.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <glm/mat4x4.hpp>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>

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

Application::Application() : window_(nullptr), ctxt_(nullptr)
{
    this_global_ptr = this;
}

Application::~Application()
{}

// TODO Add vars window width, height, name
void Application::initialize(const Variant_Map & init_params)
{
    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return;
    }    

    window_ = new Window;
    if (!window_->initialize(init_params))
        return;

    ctxt_ = new Context;
    ctxt_->initialize(init_params);
}

Application & Application::inst()
{
    return *this_global_ptr;
}

int Application::exec()
{
    // pybind11::scoped_interpreter guard{};
    // auto m = pybind11::module::import("Noble_Steed");
    // pybind11::object obj = pybind11::cast(&Context::inst());
    // m.attr("context") = obj;
    // dlog("Scooby");
    Engine * eng = ns_eng;
    eng->set_running(true);
    
    
    while (eng->is_running())
    {
        glfwPollEvents();
        eng->run_frame();
    }

    ctxt_->terminate();
    delete ctxt_;

    window_->terminate();
    delete window_;
    

    glfwTerminate();
    return 0;

    // pybind11::exec(R"(
    //     import Noble_Steed
    //     Noble_Steed.context.sys.log_internal()
    //     Noble_Steed.context.sys.internal = "poopy yo yo"
    // )");
    // ctxt.sys_.log_internal();
}

Window * Application::get_window()
{
    return window_;
}

Context * Application::get_context()
{
    return ctxt_;
}

void Application::terminate()
{
    ns_eng->set_running(false);
}

void Application::glfw_error_callback(i32 error, const char * description)
{
    elog("GLFW Error {}",description);
}

} // namespace noble_steed
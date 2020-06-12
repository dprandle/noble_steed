#include <stdio.h>
#include <bx/bx.h>
#include <bx/spscqueue.h>
#include <bx/thread.h>
#include <bx/debug.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <glm/mat4x4.hpp>
#include <iostream>
#include <sstream>

#include <noble_steed/core/context.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/core/application.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/graphics/window.h>
#include <noble_steed/core/engine.h>

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

    //glfwSetKeyCallback(window_, glfw_key_press_callback);

    window_ = new Window;
    if (!window_->initialize(init_params))
        return;

    itup2 sz = window_->get_size();

    // This will be moved to the renderer eventually
    bgfx::Init bgfx_init;
    bgfx_init.platformData.nwh = window_->get_native_window();
    bgfx_init.type = bgfx::RendererType::Count; // Automatically choose a renderer.
    bgfx_init.resolution.width = sz.w;
    bgfx_init.resolution.height = sz.h;
    bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::renderFrame();
    bgfx::init(bgfx_init);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, sz.w, sz.h);

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
        bgfx::frame();
        bgfx::touch(0);
    }

    ctxt_->terminate();
    delete ctxt_;

    bgfx::shutdown();

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
{}

} // namespace noble_steed
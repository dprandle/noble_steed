#include "bx/bx.h"
#include "bx/spscqueue.h"
#include "bx/thread.h"
#include "bx/debug.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "noble_steed/core/application.h"
#include "noble_steed/core/engine.h"
#include "noble_steed/core/context.h"
#include "noble_steed/core/resource_cache.h"

#include "noble_steed/graphics/shader.h"
#include "noble_steed/graphics/renderer.h"
#include "noble_steed/graphics/window.h"

#include "noble_steed/io/json_archive.h"
#include "noble_steed/io/input_map.h"
#include "noble_steed/io/input_translator.h"

#include "noble_steed/scene/world.h"

namespace noble_steed
{
Renderer::Renderer()
{}

Renderer::Renderer(const Renderer & copy) : System(copy)
{}

Renderer & Renderer::operator=(Renderer rhs)
{
    swap(rhs);
    return *this;
}

Renderer::~Renderer()
{}

void Renderer::render_frame()
{
    itup2 sz = app.get_window()->get_size();
    bgfx::setViewRect(0, 0, 0, sz.w, sz.h);
    bgfx::frame();
    bgfx::touch(0);
}

void Renderer::initialize(const Variant_Map & init_params)
{
    System::initialize(init_params);

    itup2 sz = app.get_window()->get_size();

    // This will be moved to the renderer eventually
    bgfx::Init bgfx_init;
    bgfx_init.platformData.nwh = app.get_window()->get_native_window();
    bgfx_init.type = bgfx::RendererType::Count; // Automatically choose a renderer.
    bgfx_init.resolution.width = sz.w;
    bgfx_init.resolution.height = sz.h;
    bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::renderFrame();
    bgfx::init(bgfx_init);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, sz.w, sz.h);

    sig_connect(ns_eng->render, this, &Renderer::render_frame);
    sig_connect(ns_eng->update, [&](){process_events();});
    subscribe_event_handler(str_hash("load_and_compile_shader"), this, &Renderer::compile_shader);
}

void Renderer::terminate()
{
    System::terminate();
    bgfx::shutdown();
}

void Renderer::swap(const Renderer & rhs)
{
    System::swap(rhs);
}

void Renderer::compile_shader(Event & ev)
{  
    Resource_Cache * rc = ns_ctxt.get_resource_cache();
    auto shdr = rc->add<Shader>("shaders/simple","data/core");
    shdr->set_vertex_source_from_file("import/basic_window/simple.vsc");
    shdr->set_fragment_source_from_file("import/basic_window/simple.fsc");
    shdr->set_varying_def_source_from_file("import/basic_window/varying.def.sc");
    shdr->save();
    shdr->compile();
    ilog("DONE");
}

} // namespace noble_steed
#include <bx/bx.h>
#include <bx/spscqueue.h>
#include <bx/thread.h>
#include <bx/debug.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

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
// Initialization parameters for the renderer
namespace init_param_key
{
namespace renderer
{
const String SHADER_PLATFROM = "shdr_platform";
const String SHADER_PROFILE = "shdr_profile";
const String SHADER_BINARY_RELATIVE_SUBDIR = "shdr_bin_rel_dir";
} // namespace renderer
} // namespace init_param_key

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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    shader_platform_ = "windows";
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
    shader_platform_ = "ios";
    #elif TARGET_OS_IPHONE
    shader_platform_ = "ios";
    #elif TARGET_OS_MAC
    shader_platform_ = "osx";
    #else
    shader_platform_ = "unknown";
    #endif
#elif __linux__
    shader_platform_ = "linux";
#elif __unix__ // all unices not caught above
    shader_platform_ = "linux";
#elif defined(_POSIX_VERSION)
    shader_platform_ = "linux";
#else
    shader_platform_ = "unsupported";
#endif

    System::initialize(init_params);

    if (!grab_param(init_params,init_param_key::renderer::SHADER_PLATFROM,shader_platform_))
    {
        wlog("No platform provided - using default detected ({})",shader_platform_);
    }

    shader_profile_ = "glsl";
    if (!grab_param(init_params,init_param_key::renderer::SHADER_PROFILE, shader_profile_))
    {
        ilog("No profile provided - using {} which is default",shader_profile_);
    }

    shader_bin_rel_dir_ = shader_profile_;
    if (!grab_param(init_params, init_param_key::renderer::SHADER_BINARY_RELATIVE_SUBDIR, shader_bin_rel_dir_))
    {
        ilog("No shader binary directory provided - using default {}", shader_bin_rel_dir_);
    }

    itup2 sz = app.get_window()->get_size();

    /// This will be moved to the renderer eventually
    bgfx::Init bgfx_init;

    bgfx_init.platformData.nwh = app.get_window()->get_native_window();
    if (shader_profile_ == "glsl")
    {
        bgfx_init.type = bgfx::RendererType::OpenGL;
    }
    else if (shader_profile_ == "spirv")
    {
        bgfx_init.type = bgfx::RendererType::Vulkan;
    }
    else if (shader_profile_ == "metal")
    {
        bgfx_init.type = bgfx::RendererType::Metal;
    }
    else if (shader_profile_.find("s_3") != String::npos)
    {
        bgfx_init.type = bgfx::RendererType::Direct3D9;
    }
    else if (shader_profile_.find("s_4") != String::npos)
    {
        bgfx_init.type = bgfx::RendererType::Direct3D11;
    }
    else if (shader_profile_.find("s_5") != String::npos)
    {
        bgfx_init.type = bgfx::RendererType::Direct3D12;
    }
    else
    {
        bgfx_init.type = bgfx::RendererType::Gnm;
    }
    

    bgfx_init.resolution.width = sz.w;
    bgfx_init.resolution.height = sz.h;
    bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::renderFrame();
    bgfx::init(bgfx_init);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, sz.w, sz.h);

    sig_connect(ns_eng->render, this, &Renderer::render_frame);
    sig_connect(ns_eng->update, [&]() { process_events(); });
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
    auto shdr = rc->add<Shader>("shaders/simple", "data/core");
    if (!shdr)
        shdr = rc->get<Shader>("shaders/simple", "data/core");
    shdr->set_vertex_source_from_file("import/basic_window/simple.vsc");
    shdr->set_fragment_source_from_file("import/basic_window/simple.fsc");
    shdr->set_varying_def_source_from_file("import/basic_window/varying.def.sc");
    shdr->save();
    shdr->compile(shader_platform_, shader_profile_);
    shdr->create_program(shader_bin_rel_dir_);
}

} // namespace noble_steed
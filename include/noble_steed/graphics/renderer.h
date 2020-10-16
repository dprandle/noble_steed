#include <bgfx/bgfx.h>
#include "../core/system.h"

namespace noble_steed
{

/// Initialization parameters for the renderer
namespace init_param_key
{
namespace renderer
{
/// string - Which platform should the shader compiler use - options are android/asm.js/ios/linux/orbis/osx/windows
extern const String SHADER_PLATFROM;

/// string - Shader profile to use as given by bgfx - see the shaderc documentation. Would list here, but if it changes then well - I would
/// have to change this which is no good. If this is empty or missing, glsl will be used.
extern const String SHADER_PROFILE;

/// string - Shader binary relative subdirectory - this is where all the compiled shaders will be loaded from within the shader resource directory. For example,
/// if a shader is in the package "core/" and its name is shaders/test and the SHADER_BINARY_RELATIVE_SUBDIR is set to "glsl", then the full binary shader
/// file loaded in (or tried to load in) would be core/shaders/test/glsl/vertex_test.shbin and core/shaders/test/glsl/fragment_test.shbin
/// If this 
extern const String SHADER_BINARY_RELATIVE_SUBDIR;
} // namespace context
} // namespace init_param_key


class Renderer : public System
{
    RTTR_ENABLE(System)
  public:
    Renderer();

    Renderer(const Renderer & copy);

    Renderer & operator=(Renderer rhs);

    virtual ~Renderer();

    void render_frame();

    void initialize(const Variant_Hash & init_params);

    void terminate();
  protected:

    void compile_shader(Event & ev);

    /// Event handler for resize framebuffer event - called every time the framebuffer is resized either programmatically or by the user resizing the window
    void handle_framebuffer_resize(Event & ev);


    void register_default_types_();
    void swap(const Renderer & rhs);

  private:
    String shader_platform_;
    String shader_profile_;
    String shader_bin_rel_dir_;

};

} // namespace noble_steed
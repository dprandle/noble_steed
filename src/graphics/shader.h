#pragma once

#include <bgfx/bgfx.h>

#include "../core/resource.h"
#include "../container/hash_set.h"

namespace noble_steed
{

class Shader : public Resource
{
  public:
    Shader();

    Shader(const Shader &copy);

    virtual ~Shader();

    /// Compile shader to byte code that the render that can then be used in create_program
    /// Platform and shader_model_profile should be an option matching shaderc --platform and --profile
    /// If output_relative_dir is not specified, shader_model_profile is used as the output dir
    bool compile(const String &platform, const String &shader_model_profile, const String &output_relative_dir = "");

    /// Creates a program from a binary shader file - this will crash if the wrong outur_relative_dir for the renderer type is passed in
    /// Meaning, if the renderer type is set to metal and glsl is passed in - this will crash. Shader will do what its told though and try.
    bool create_program(const String &output_relative_dir);

    void initialize(const Variant_Map &init_params);

    void terminate();

    Shader &operator=(Shader rhs);

    const String &get_vertex_source();

    const String &get_fragment_source();

    const String &get_varying_defines();

    bool set_vertex_source_from_file(const String &fname);

    bool set_fragment_source_from_file(const String &fname);

    bool set_varying_def_source_from_file(const String &fname);

    void set_vertex_source(const String &source);

    void set_fragment_source(const String &source);

    void set_varying_defines(const String &source);

    void set_binary_extension(const String &ext);

    const String &get_binary_extension();

  protected:
    virtual void swap(Shader &rhs);

  private:
    bool build_shader_(const String &rel_base_name,
                       const String &type,
                       const String &source,
                       const String &platform,
                       const String &shader_model_profile,
                       const String &output_relative_dir);
    bool load_shader_(const String &fname, bgfx::ShaderHandle &handle);
    bgfx::ProgramHandle prog_handle_;

    String vertex_src_;
    String fragment_src_;
    String varying_def_src_;
    String binary_extension_;
    hash_set<String> extra_include_dirs_;
};

} // namespace noble_steed
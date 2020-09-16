#pragma once

#include "../core/common.h"
#include "../core/resource.h"
#include "bgfx/bgfx.h"

namespace noble_steed
{
class Shader : public Resource
{
  public:
    Shader();

    Shader(const Shader & copy);

    virtual ~Shader();

    bool compile();

    bool create_program();

    void initialize(const Variant_Map & init_params);

    void terminate();

    Shader & operator=(Shader rhs);

    const String & get_vertex_source();

    const String & get_fragment_source();

    const String & get_varying_defines();

    bool set_vertex_source_from_file(const String & fname);

    bool set_fragment_source_from_file(const String & fname);

    bool set_varying_def_source_from_file(const String & fname);

    void set_vertex_source(const String & source);

    void set_fragment_source(const String & source);

    void set_varying_defines(const String & source);

    void set_binary_extension(const String & ext);

    const String & get_binary_extension();

  protected:
    virtual void swap(Shader & rhs);

    virtual void pack_begin(JSON_Archive::Direction io_dir);

    virtual void pack_end(JSON_Archive::Direction io_dir);

  private:
    bool build_shader_(const String & rel_base_name, const String & type, const String & source, String & output_name);
    bool load_shader_(const String & fname, bgfx::ShaderHandle & handle);
    bgfx::ProgramHandle prog_handle_;

    String vertex_src_;
    String fragment_src_;
    String varying_def_src_;
    String binary_extension_;
    Hash_Set<String> extra_include_dirs_;

    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(Resource)
};

} // namespace noble_steed
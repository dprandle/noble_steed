#include "shader.h"
#include "../io/filesystem.h"
#include "../io/logger.h"
#include "../core/context.h"

namespace noble_steed
{
const String SHADER_LIB_LOCATION = "data/shader_lib";

Shader::Shader() : Resource(), prog_handle_(), vertex_src_(), fragment_src_(), varying_def_src_(), binary_extension_(".shbin")
{}

Shader::Shader(const Shader &copy) : Resource(copy)
{}

Shader::~Shader()
{}

void Shader::initialize(const Variant_Map &init_params)
{
    Resource::initialize(init_params);
}

void Shader::terminate()
{
    Resource::terminate();
}

bool Shader::compile(const String &platform, const String &shader_model_profile, const String &output_relative_dir)
{
    bool ret = false;
    String outdir(output_relative_dir);

    if (outdir.empty())
        outdir = shader_model_profile;

    // First, create text files from each src string (if not empty)
    String containing_dir = get_relative_containing_dir_path();
    String varying_fname = containing_dir + "varying.def.sc";
    if (!fs::write_string_to_file(varying_fname, varying_def_src_))
    {
        wlog("Could not write varying def {} to file - file could not be opened", varying_fname);
        return ret;
    }
    String profile = shader_model_profile;
    if (profile == "glsl")
        profile = "";
    ret = build_shader_(containing_dir, "vertex", vertex_src_, platform, profile, outdir);
    ret = ret && build_shader_(containing_dir, "fragment", fragment_src_, platform, profile, outdir);
    fs::remove(varying_fname);
    return ret;
}

bool Shader::load_shader_(const String &fname, bgfx::ShaderHandle &handle)
{
    i8_vector buf;
    if (!fs::read_file_to_buffer(fname, buf))
    {
        wlog("Could not open {}", fname);
        return false;
    }
    buf.push_back(0);
    const bgfx::Memory *mem = bgfx::copy(buf.data(), buf.size());
    handle = bgfx::createShader(mem);
    return true;
}

bool Shader::build_shader_(const String &containing_dir,
                           const String &type,
                           const String &source,
                           const String &platform,
                           const String &shader_model_profile,
                           const String &output_relative_dir)
{
    String basename = get_basename() + "_" + type;
    String fname = containing_dir + basename + ".sc";
    String output_dir = containing_dir + output_relative_dir;
    fs::create_directory(output_dir);
    String output_name = output_dir + "/" + basename + binary_extension_;
    if (!fs::write_string_to_file(fname, source))
    {
        wlog("Could not write {} shader {} to file - file could not be opened", type, fname);
        return false;
    }

    String output;
    vector<String> args;
    args.push_back("-f");
    args.push_back(fname);
    args.push_back("-o");
    args.push_back(output_name);
    args.push_back("--platform");
    args.push_back(platform);
    args.push_back("--type");
    args.push_back(type);
    args.push_back("--profile");
    args.push_back(shader_model_profile);
    args.push_back("--verbose");
    args.push_back("-i");
    args.push_back(SHADER_LIB_LOCATION);
    fs::fork_and_wait(COMMAND_LINE_TOOLS_DIR + "/shaderc", args, output);
    fs::remove(fname);
    if (output.find("Error") != String::npos || output.find("Failed") != String::npos)
    {
        wlog("Failed to compile {} shader from {} to {}:\n{}", type, fname, basename + binary_extension_, output);
        return false;
    }
    dlog("Compiled {} shader from {} to {}:\n{}", type, fname, output_name, output);
    return true;
}

bool Shader::create_program(const String &output_relative_dir)
{
    bool ret = true;
    String prefix = get_relative_containing_dir_path() + output_relative_dir + "/" + get_basename();
    String vert_out = prefix + "_vertex" + binary_extension_;
    String frag_out = prefix + "_fragment" + binary_extension_;

    bgfx::ShaderHandle frag_handle, vert_handle;
    ret = ret && load_shader_(vert_out, vert_handle);
    ret = ret && load_shader_(frag_out, frag_handle);
    if (ret)
    {
        prog_handle_ = bgfx::createProgram(vert_handle, frag_handle, true);
    }
    else
    {
        if (vert_handle.idx != 0)
            bgfx::destroy(vert_handle);
        if (frag_handle.idx != 0)
            bgfx::destroy(frag_handle);
    }
    return ret;
    return ret;
}

Shader &Shader::operator=(Shader rhs)
{
    swap(rhs);
    return *this;
}

bool Shader::set_vertex_source_from_file(const String &fname)
{
    vertex_src_.clear();
    if (fs::read_file_to_string(fname, vertex_src_))
    {
        ilog("Read vertex source from {}", fname);
        return true;
    }
    return false;
}

bool Shader::set_fragment_source_from_file(const String &fname)
{
    fragment_src_.clear();
    if (fs::read_file_to_string(fname, fragment_src_))
    {
        ilog("Read fragment source from {}", fname);
        return true;
    }
    return false;
}

bool Shader::set_varying_def_source_from_file(const String &fname)
{
    varying_def_src_.clear();
    if (fs::read_file_to_string(fname, varying_def_src_))
    {
        ilog("Read varying def source from {}", fname);
        return true;
    }
    return false;
}

const String &Shader::get_vertex_source()
{
    return vertex_src_;
}

const String &Shader::get_fragment_source()
{
    return fragment_src_;
}

const String &Shader::get_varying_defines()
{
    return varying_def_src_;
}

void Shader::set_vertex_source(const String &source)
{
    vertex_src_ = source;
}

void Shader::set_fragment_source(const String &source)
{
    fragment_src_ = source;
}

void Shader::set_varying_defines(const String &source)
{
    varying_def_src_ = source;
}

void Shader::set_binary_extension(const String &ext)
{
    binary_extension_ = ext;
}

const String &Shader::get_binary_extension()
{
    return binary_extension_;
}

void Shader::swap(Shader &rhs)
{
    Resource::swap(rhs);
}

} // namespace noble_steed
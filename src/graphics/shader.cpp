#include "noble_steed/graphics/shader.h"
#include "noble_steed/io/filesystem.h"
#include "noble_steed/io/logger.h"
#include "noble_steed/core/context.h"

namespace noble_steed
{
const String SHADER_LIB_LOCATION = "data/shader_lib";

Shader::Shader() : Resource(), prog_handle_(), vertex_src_(), fragment_src_(), varying_def_src_(), binary_extension_(".shbin")
{}

Shader::Shader(const Shader & copy) : Resource(copy)
{}

Shader::~Shader()
{}

void Shader::initialize(const Variant_Map & init_params)
{
    Resource::initialize(init_params);
}

void Shader::terminate()
{
    Resource::terminate();
}

bool Shader::compile()
{
    bool ret = false;
    // First, create text files from each src string (if not empty)
    String rel_base_name = get_relative_basename();
    String varying_fname = get_relative_containing_dir_path() + "varying.def.sc";
    if (!fs::write_string_to_file(varying_fname, varying_def_src_))
    {
        wlog("Could not write varying def {} to file - file could not be opened", varying_fname);
        return ret;
    }
    String frag_out, vert_out;
    bgfx::ShaderHandle frag_handle, vert_handle;
    ret = build_shader_(rel_base_name, "vertex", vertex_src_, vert_out);
    ret = ret && build_shader_(rel_base_name, "fragment", fragment_src_, frag_out);
    ret = ret && load_shader_(vert_out,vert_handle);
    ret = ret && load_shader_(frag_out,frag_handle);
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
    fs::remove(varying_fname);
    return ret;
}

bool Shader::load_shader_(const String & fname, bgfx::ShaderHandle & handle)
{
    i8_Vector buf;
    if (!fs::read_file_to_buffer(fname, buf))
    {
        wlog("Could not open {}",fname);
        return false;
    }
    buf.push_back(0);
    const bgfx::Memory * mem = bgfx::copy(buf.data(), buf.size());
    handle = bgfx::createShader(mem);
    return true;
}

bool Shader::build_shader_(const String & rel_base_name, const String & type, const String & source, String & output_name)
{
    String basename = rel_base_name + "_" + type;
    String fname = basename + ".sc";
    output_name = basename + binary_extension_;
    if (!fs::write_string_to_file(fname, source))
    {
        wlog("Could not write {} shader {} to file - file could not be opened", type, fname);
        return false;
    }

    String output;
    Vector<String> args;
    args.push_back("-f");
    args.push_back(fname);
    args.push_back("-o");
    args.push_back(output_name);
    args.push_back("--platform");
    args.push_back("osx");
    args.push_back("--type");
    args.push_back(type);
    args.push_back("--profile");
    args.push_back("metal");
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
    dlog("Compiled {} shader from {} to {}:\n{}", type, fname, basename + binary_extension_, output);
    return true;
}

bool Shader::create_program()
{
    return true;
}

Shader & Shader::operator=(Shader rhs)
{
    swap(rhs);
    return *this;
}

bool Shader::set_vertex_source_from_file(const String & fname)
{
    vertex_src_.clear();
    if (fs::read_file_to_string(fname, vertex_src_))
    {
        ilog("Read vertex source from {}", fname);
        return true;
    }
    return false;
}

bool Shader::set_fragment_source_from_file(const String & fname)
{
    fragment_src_.clear();
    if (fs::read_file_to_string(fname, fragment_src_))
    {
        ilog("Read fragment source from {}", fname);
        return true;
    }
    return false;
}

bool Shader::set_varying_def_source_from_file(const String & fname)
{
    varying_def_src_.clear();
    if (fs::read_file_to_string(fname, varying_def_src_))
    {
        ilog("Read varying def source from {}", fname);
        return true;
    }
    return false;
}

const String & Shader::get_vertex_source()
{
    return vertex_src_;
}

const String & Shader::get_fragment_source()
{
    return fragment_src_;
}

const String & Shader::get_varying_defines()
{
    return varying_def_src_;
}

void Shader::set_vertex_source(const String & source)
{
    vertex_src_ = source;
}

void Shader::set_fragment_source(const String & source)
{
    fragment_src_ = source;
}

void Shader::set_varying_defines(const String & source)
{
    varying_def_src_ = source;
}

void Shader::set_binary_extension(const String & ext)
{
    binary_extension_ = ext;
}

const String & Shader::get_binary_extension()
{
    return binary_extension_;
}

void Shader::swap(Shader & rhs)
{
    Resource::swap(rhs);
}

void Shader::pack_begin(JSON_Archive::Direction io_dir)
{}

void Shader::pack_end(JSON_Archive::Direction io_dir)
{}
} // namespace noble_steed
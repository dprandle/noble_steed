#include "noble_steed/graphics/shader.h"
#include "noble_steed/io/filesystem.h"
#include "noble_steed/io/logger.h"

namespace noble_steed
{
Shader::Shader() : Resource()
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
    // First, create text files from each src string (if not empty)
    String vert_nm = get_relative_basename() + ".vsc";
    String frag_nm = get_relative_basename() + ".fsc";
    String vary_nm = get_relative_basename() + ".def";
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
    if (fs::read_file_to_string(fname,vertex_src_))
    {
        ilog("Read vertex source from {}",fname);
        return true;
    }
    return false;
}

bool Shader::set_fragment_source_from_file(const String & fname)
{
    fragment_src_.clear();
    if (fs::read_file_to_string(fname,fragment_src_))
    {
        ilog("Read fragment source from {}",fname);
        return true;
    }
    return false;
}

bool Shader::set_varying_def_source_from_file(const String & fname)
{
    varying_def_src_.clear();
    if (fs::read_file_to_string(fname,varying_def_src_))
    {
        ilog("Read varying def source from {}",fname);
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

void Shader::swap(Shader & rhs)
{
    Resource::swap(rhs);
}

void Shader::pack_begin(JSON_Archive::Direction io_dir)
{}

void Shader::pack_end(JSON_Archive::Direction io_dir)
{}
} // namespace noble_steed
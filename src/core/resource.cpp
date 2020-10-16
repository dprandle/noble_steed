#include <functional>

#include <noble_steed/core/resource.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/io/json_archive.h>
#include <noble_steed/core/context.h>
#include <noble_steed/io/filesystem.h>

namespace noble_steed
{
const String CORE_PACKAGE_NAME = "data/core";
const String NONE_LOADED_PACKAGE_NAME = "data/scooby";

Resource::Resource() : Context_Obj(), package_(), name_(), display_name_(), id_(0)
{}

Resource::Resource(const Resource & copy) : Context_Obj(copy), package_(copy.package_), display_name_(copy.display_name_), id_(0)
{
    set_name(copy.name_ + " (copy)");
}

Resource & Resource::operator=(Resource rhs)
{
    swap(rhs);
    return *this;
}

void Resource::swap(Resource & rhs)
{
    Context_Obj::swap(rhs);
    std::swap(package_, rhs.package_);
    std::swap(name_, rhs.name_);
    std::swap(display_name_, rhs.display_name_);
    std::swap(id_, rhs.id_);
}

Resource::~Resource()
{}

bool Resource::save()
{
    return save(get_relative_path());
}

// Save data to custom file path - no regards to naming, packages, or extensions.
bool Resource::save(const String & custom_path)
{
    String str;

    // Create directories if needed...
    sizet pos = custom_path.find_last_of('/');
    if (pos != String::npos)
    {
        String dir = custom_path.substr(0, pos);
        if (!fs::is_directory(dir) && fs::create_directories(dir))
        {
            ilog("Created directory {} for {}", dir, custom_path);
        }
    }

    std::ofstream file(custom_path);
    if (file)
    {
        str = to_json();
        file << str;
        ilog("Saved {} in package {} to {}", name_, package_, custom_path);
        return true;
    }
    else
    {
        wlog("Could not open file {}", custom_path);
    }
    return false;
}

bool Resource::load(const String & custom_path)
{
    std::ifstream file(custom_path);
    std::stringstream buffer;
    if (file)
    {
        buffer << file.rdbuf();
        from_json(buffer.str());
        ilog("Loaded {} in package {} from {}", name_, package_, custom_path);
        return true;
    }
    else
    {
        wlog("Could not open file {}", custom_path);
    }
    return false;
}

bool Resource::load()
{
    return load(get_relative_path());
}

void Resource::initialize(const Variant_Hash & init_params)
{
    ilog("Initializing resource type {}", String(get_derived_info().m_type.get_name()));
}

void Resource::terminate()
{
    ilog("Terminating resource type {}", String(get_derived_info().m_type.get_name()));
}

String Resource::get_relative_path()
{
    return package_ + get_subdir_path();
}

String Resource::get_basename()
{
    fs::path pth = get_filename();
    return pth.stem();
}

String Resource::get_relative_containing_dir_path()
{
    return package_ + get_subdir_dir();
}

String Resource::get_subdir_dir()
{
    fs::path pth = get_subdir_path();
    return pth.remove_filename();
}

String Resource::get_subdir_path()
{
    String ext = ns_ctxt.get_resource_extension(get_derived_info().m_type);
    return name_ + ext;
}

String Resource::get_filename()
{
    fs::path pth = get_subdir_path();
    return pth.filename();
}

const String & Resource::get_name()
{
    return name_;
}

String Resource::get_relative_basename()
{
    return package_ + name_;
}

const String & Resource::get_package()
{
    return package_;
}

u32 Resource::get_id()
{
    return id_;
}

const String & Resource::get_display_name()
{
    return display_name_;
}

void Resource::set_name(const String & name)
{
    String total_path = package_ + name;
    u32 hashed_id = str_hash(total_path);

    if (id_ != -1 && hashed_id != id_)
    {
        String type_str(get_derived_info().m_type.get_name());
        bool do_change = true;
        change_id(id_, hashed_id, &do_change);
        if (do_change)
        {
            dlog("{} name changed from {} to {}", type_str, name_, name);
            id_ = hashed_id;
            name_ = name;
        }
    }
}

void Resource::set_display_name(const String & disp_name)
{
    display_name_ = disp_name;
}

void Resource::set_package(const String & package_name)
{
    String total_path = package_name + name_;
    u32 hashed_id = str_hash(total_path);
    if (id_ != -1 && hashed_id != id_)
    {
        String type_str(get_derived_info().m_type.get_name());
        bool do_change = true;
        change_id(id_, hashed_id, &do_change);
        if (do_change)
        {
            dlog("{} package changed from {} to {}", type_str, package_, package_name);
            id_ = hashed_id;
            package_ = package_name;
        }
    }
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Resource>("noble_steed::Resource")
        .property("name", &Resource::get_name, &Resource::set_name)
        .property("package", &Resource::get_package, &Resource::set_package)
        .property("display_name", &Resource::get_display_name, &Resource::set_display_name);
}

#include <functional>
#include <noble_steed/core/resource.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/serialization/json_archive.h>
#include <noble_steed/core/context.h>

#include <noble_steed/core/filesystem.h>

namespace noble_steed
{
Resource::Resource()
{}

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
        String dir = custom_path.substr(0,pos);
        if (!fs::is_directory(dir) && fs::create_directories(dir))
        {
            ilog("Created directory {} for {}",dir,custom_path);
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
    String str;
    if (file)
    {
        file >> str;
        from_json(str);
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

void Resource::initialize(const Variant_Map & init_params)
{
    ilog("Initializing resource type {}", String(get_derived_info().m_type.get_name()));
}

void Resource::terminate()
{
    ilog("Terminating resource type {}", String(get_derived_info().m_type.get_name()));
}

String Resource::get_relative_path()
{
    String ext = ns_ctxt.get_resource_extension(get_derived_info().m_type);
    return package_ + name_ + ext;
}

const String & Resource::get_name()
{
    return name_;
}

const String & Resource::get_package()
{
    return package_;
}

u64 Resource::get_id()
{
    return id_;
}

const String & Resource::get_display_name()
{
    return display_name_;
}

void Resource::set_name(const String & name)
{
    std::hash<String> hasher;
    String total_path = package_ + name;
    u64 hashed_id = hasher(total_path);
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
    std::hash<String> hasher;
    String total_path = package_name + name_;
    u64 hashed_id = hasher(total_path);
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

    registration::class_<Resource>("Resource")
        .constructor<>()
        .method("save_json", select_overload<bool(void)>(&Resource::save), registration::public_access)
        .method("load_json", select_overload<bool(void)>(&Resource::load), registration::public_access)
        .method("save_json", select_overload<bool(const String &)>(&Resource::save), registration::public_access)
        .method("load_json", select_overload<bool(const String &)>(&Resource::load), registration::public_access)
        .method("pack_unpack", &Resource::pack_unpack, registration::public_access)
        .method("initialize", &Resource::initialize, registration::public_access)
        .method("terminate", &Resource::terminate, registration::public_access)
        .method("get_relative_path", &Resource::get_relative_path, registration::public_access)
        .method("get_id", &Resource::get_id, registration::public_access)
        .property("name", &Resource::get_name, &Resource::set_name, registration::public_access)
        .property("package", &Resource::get_package, &Resource::set_package, registration::public_access)
        .property("display_name", &Resource::get_display_name, &Resource::set_display_name, registration::public_access);
}

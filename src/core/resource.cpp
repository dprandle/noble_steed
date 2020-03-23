#include <functional>
#include <noble_steed/core/resource.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/serialization/json_archive.h>

namespace noble_steed
{
Resource::Resource()
{}

Resource::~Resource()
{}

void Resource::save()
{
    dlog("Calling base save");
    JSON_Archive ar;
    ar.io_dir = JSON_Archive::DIR_OUT;
    pack_unpack(ar);
    dlog("Saved:\n{}", ar.json_str);
}

void Resource::load()
{
    dlog("Calling base load");
    JSON_Archive ar;
    ar.io_dir = JSON_Archive::DIR_IN;
    pack_unpack(ar);
}

void Resource::pack_unpack(JSON_Archive & ar)
{
    dlog("Calling pack unpack in base");
    noble_steed::pack_unpack(ar, *this);
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
    return package_ + name_;
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
        dlog("{} name changed from {} to {}", type_str, name_, name);
        change_id(id_, hashed_id);
        id_ = hashed_id;
        name_ = name;
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
        dlog("{} package changed from {} to {}", type_str, package_, package_name);
        change_id(id_, hashed_id);
        id_ = hashed_id;
        package_ = package_name;
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
        .method("save", &Resource::save, registration::public_access)
        .method("load", &Resource::load, registration::public_access)
        .method("pack_unpack", &Resource::pack_unpack, registration::public_access)
        .method("initialize", &Resource::initialize, registration::public_access)
        .method("terminate", &Resource::terminate, registration::public_access)
        .method("get_relative_path", &Resource::get_relative_path, registration::public_access)
        .method("get_id", &Resource::get_id, registration::public_access)
        .property("name", &Resource::get_name, &Resource::set_name, registration::public_access)
        .property("package", &Resource::get_package, &Resource::set_package, registration::public_access)
        .property("display_name", &Resource::get_display_name, &Resource::set_display_name, registration::public_access);
}

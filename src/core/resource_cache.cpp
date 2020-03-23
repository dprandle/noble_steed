#include <noble_steed/core/resource_cache.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/core/resource.h>
#include <noble_steed/core/context.h>
#include <noble_steed/core/filesystem.h>

namespace noble_steed
{

const String PACKAGE_DIRS_KEY = "Package_Dirs";
const String CURRENT_PACAKGE_KEY = "Current_Package";
const String CORE_PACKAGE_NAME = "data/core";
const String NONE_LOADED_PACKAGE_NAME = "data/scooby";

Resource_Cache::Resource_Cache()
{}

Resource_Cache::~Resource_Cache()
{}

Resource * Resource_Cache::add(const rttr::type & resource_type, const String & name, const String & package, const Variant_Map & init_params)
{
    Resource * resource = allocate_resource_(resource_type);
    if (!add_resource_(resource, name, package, init_params))
    {
        deallocate_resource_(resource, resource_type);
        return nullptr;
    }
    return resource;
}

Resource * Resource_Cache::add(const Resource & copy, const String & name, const String & package)
{
    rttr::type t = const_cast<Resource &>(copy).get_derived_info().m_type;
    Resource * resource = allocate_resource_(t, copy);
    if (!add_resource_(resource, name, package, Variant_Map()))
    {
        deallocate_resource_(resource, t);
        return nullptr;
    }
    return resource;
}

Resource * Resource_Cache::get(const String & name, const String & package)
{
    String actual_package(package);
    if (actual_package.empty())
        actual_package = current_package_;

    if (actual_package.empty())
    {
        wlog("Cannot load resource {} - no package passed in and no current package set", name);
    }
    std::hash<String> hash;
    u64 id = hash(package + name);
    return get(id);
}

Resource * Resource_Cache::get(u64 id)
{
    auto fiter = resources_.find(id);
    if (fiter != resources_.end())
        return fiter->second;
    return nullptr;
}

bool Resource_Cache::remove(u64 id)
{
    Resource * ret = nullptr;
    auto fiter = resources_.find(id);
    if (fiter != resources_.end())
    {
        ret = fiter->second;
        ret->terminate();
        fiter = resources_.erase(fiter);
        deallocate_resource_(ret, ret->get_derived_info().m_type);
        return true;
    }
    else
    {
        wlog("Could not remove resource with id {} from Resource Cache as the id wasn't found", id);
        return false;
    }
}

bool Resource_Cache::remove(const String & name, const String & package)
{
    String actual_package(package);
    if (actual_package.empty())
        actual_package = current_package_;

    if (actual_package.empty())
    {
        wlog("Cannot load resource {} - no package passed in and no current package set", name);
    }

    std::hash<String> hash;
    u64 id = hash(package + name);
    return remove(id);
}

bool Resource_Cache::remove(Resource * resource)
{
    return remove(resource->get_id());
}

void Resource_Cache::initialize(const Variant_Map & init_params)
{
    ilog("Initializing resource cache");

    auto iter = init_params.find(PACKAGE_DIRS_KEY);
    if (iter != init_params.end())
    {
        if (iter->second.is_type<Vector<String>>())
        {
            auto view = iter->second.create_sequential_view();
            for (int i = 0; i < view.get_size(); ++i)
                load_package(view.get_value(i).to_string(), false);
        }
        {
            wlog("Passed in correct key {} but incorrect value type:{}", iter->first, String(iter->second.get_type().get_name()));
        }
    }

    iter = init_params.find(CURRENT_PACAKGE_KEY);
    if (iter != init_params.end())
    {
        if (iter->second.is_type<String>())
        {
            set_current_package(iter->second.get_value<String>());
        }
        else
        {
            wlog("Passed in correct key {} but incorrect value type:{}", iter->first, String(iter->second.get_type().get_name()));
        }
    }

    if (loaded_packages_.empty())
    {
        load_package("data/scooby", true);
    }
    load_package("data/core", false);
}

void Resource_Cache::terminate()
{
    ilog("Terminating resource cache");
}

void Resource_Cache::make_valid_package_name_(String & str)
{
    if (str.find_last_of('/') != str.size() - 1)
    {
        str.push_back('/');
    }
}

bool Resource_Cache::load_package(String package, bool make_current)
{
    make_valid_package_name_(package);
    auto inserted = loaded_packages_.emplace(package);
    if (inserted.second)
    {
        
        //fs::create_directory(fs::current_path() + package);

        // Load all resources recursively in this directory!

        if (make_current)
            set_current_package(package);

        return true;
    }
    return false;
}

void Resource_Cache::unload_package(String package)
{
    make_valid_package_name_(package);
}

void Resource_Cache::set_current_package(String package)
{
    make_valid_package_name_(package);
    auto fiter = loaded_packages_.find(package);
    if (fiter != loaded_packages_.end())
    {
        current_package_ = package;
    }
    else
    {
        wlog("Cannot set package {} to current package - it's not loaded - load the package first!", package);
    }
}

Resource * Resource_Cache::allocate_resource_(const rttr::type & type)
{
    Resource_Factory * fac = ns_ctxt.get_factory<Resource_Factory>(type);
    assert(fac != nullptr);
    Resource * res = fac->create();
    assert(res != nullptr);
    return res;
}

Resource * Resource_Cache::allocate_resource_(const rttr::type & type, const Resource & copy)
{
    Resource_Factory * fac = ns_ctxt.get_factory<Resource_Factory>(type);
    assert(fac != nullptr);
    Resource * res = fac->create(copy);
    assert(res != nullptr);
    return res;
}

bool Resource_Cache::add_resource_(Resource * res, const String & name, const String & package, const Variant_Map & init_params)
{
    String actual_package(package);
    if (actual_package.empty())
        actual_package = current_package_;

    if (actual_package.empty())
    {
        wlog("Cannot load resource {} - no package passed in and no current package set", name);
    }

    res->set_name(name);
    res->set_package(actual_package);
    auto fiter = resources_.emplace(res->get_id(), res);
    if (fiter.second)
    {
        res->initialize(init_params);
    }
    else
    {
        wlog("Could not add resource type {} with name {} to package {} as the resource id "
             "already exists",
             String(res->get_derived_info().m_type.get_name()),
             name,
             package);
    }
    return fiter.second;
}

void Resource_Cache::deallocate_resource_(Resource * res, const rttr::type & resource_type)
{
    assert(res != nullptr);
    res->~Resource();
    PoolAllocator * alloc = ns_ctxt.get_resource_allocator(resource_type);
    ilog("De-allocating {0} bytes for {1}", resource_type.get_sizeof(), String(resource_type.get_name()));
    alloc->Free(res);
}
} // namespace noble_steed
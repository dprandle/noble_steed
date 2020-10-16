#include <noble_steed/core/resource_cache.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/core/resource.h>
#include <noble_steed/core/context.h>
#include <noble_steed/io/filesystem.h>

namespace noble_steed
{
namespace init_param_key
{
namespace resource_cache
{
const String PACKAGE_DIRS = "Package_Dirs";
const String CURRENT_PACKAGE = "Current_Package";
} // namespace resource_cache
} // namespace init_params

Resource_Cache::Resource_Cache()
{}

Resource_Cache::~Resource_Cache()
{
    clear();
}

void Resource_Cache::initialize(const Variant_Hash & init_params)
{
    ilog("Initializing resource cache");
    using namespace init_param_key::resource_cache;

    auto iter = init_params.find(PACKAGE_DIRS);
    if (iter != init_params.end())
    {
        if (iter->second.is_type<Vector<String>>())
        {
            // auto view = iter->second.create_sequential_view();
            // for (int i = 0; i < view.get_size(); ++i)
            //     load_package(view.get_value(i).to_string(), false);
        }
        else
        {
            //wlog("Passed in correct key {} but incorrect value type:{}", iter->first, String(iter->second.get_type().get_name()));
        }
    }

    iter = init_params.find(CURRENT_PACKAGE);
    if (iter != init_params.end())
    {
        if (iter->second.is_type<String>())
        {
            set_current_package(iter->second.get_value<String>());
        }
        else
        {
            //wlog("Passed in correct key {} but incorrect value type:{}", iter->first, String(iter->second.get_type().get_name()));
        }
    }

    if (loaded_packages_.empty())
    {
        load_package(NONE_LOADED_PACKAGE_NAME, true);
    }
    load_package(CORE_PACKAGE_NAME, false);
}

void Resource_Cache::terminate()
{
    ilog("Terminating resource cache");
    clear();
}

Resource * Resource_Cache::add(const rttr::type & resource_type, const String & name, const String & package, const Variant_Hash & init_params)
{
    return add(type_hash(resource_type), name, package, init_params);
}

Resource * Resource_Cache::add(u32 type_id, const String & name, const String & package, const Variant_Hash & init_params)
{
    Resource * resource = allocate_resource_(type_id);
    if (!add_resource_(resource, name, package, init_params))
    {
        deallocate_resource_(resource, type_id);
        return nullptr;
    }
    return resource;
}

Resource * Resource_Cache::load(const rttr::type & resource_type, const String & name, const String & package, const Variant_Hash & init_params)
{
    return load(type_hash(resource_type), name, package, init_params);
}

Resource * Resource_Cache::load(u32 type_id, const String & name, const String & package, const Variant_Hash & init_params)
{
    Resource * res = add(type_id, name, package, init_params);
    if (res != nullptr && !res->load())
    {
        remove(res);
        res = nullptr;
    }
    return res;
}

Resource * Resource_Cache::load(const rttr::type & resource_type,
                                const String & name,
                                const String & package,
                                const String & custom_path,
                                const Variant_Hash & init_params)
{
    Resource * res = add(resource_type, name, package, init_params);
    if (res != nullptr && !res->load(custom_path))
    {
        remove(res);
        res = nullptr;
    }
    return res;
}

bool Resource_Cache::save(const String & custom_path, u32 resource_id) const
{
    Resource * res = get(resource_id);
    if (res != nullptr)
        return res->save(custom_path);
    return false;
}

bool Resource_Cache::save(const String & custom_path, const String & res_name, const String & res_package) const
{
    Resource * res = get(res_name, res_package);
    if (res != nullptr)
        return res->save(custom_path);
    return false;
}

bool Resource_Cache::save(u32 resource_id) const
{
    Resource * res = get(resource_id);
    if (res != nullptr)
        return res->save();
    return false;
}

bool Resource_Cache::save(const String & res_name, const String & res_package) const
{
    Resource * res = get(res_name, res_package);
    if (res != nullptr)
        return res->save();
    return false;
}

Resource * Resource_Cache::add_from_(const Resource & copy)
{
    rttr::type t = const_cast<Resource &>(copy).get_derived_info().m_type;
    u32 type_id = type_hash(t);
    Resource * resource = allocate_resource_(type_id, copy);
    if (!add_resource_(resource, resource->get_name(), resource->get_package(), Variant_Hash()))
    {
        deallocate_resource_(resource, type_id);
        return nullptr;
    }
    return resource;
}

Resource * Resource_Cache::get(const String & name, const String & package) const
{
    String actual_package(package);
    if (actual_package.empty())
        actual_package = current_package_;
    else
        make_valid_package_name_(actual_package);

    if (actual_package.empty())
    {
        wlog("Cannot load resource {} - no package passed in and no current package set", name);
    }
    u32 id = str_hash(actual_package + name);
    return get(id);
}

Resource * Resource_Cache::get(u32 id) const
{
    auto fiter = resources_.find(id);
    if (fiter != resources_.end())
        return fiter->second;
    return nullptr;
}

Vector<Resource *> Resource_Cache::get_all() const
{
    Vector<Resource *> ret;
    ret.resize(resources_.size());

    sizet ind = 0;
    auto iter = resources_.begin();
    while (iter != resources_.end())
    {
        ret[ind] = iter->second;
        ++ind;
        ++iter;
    }
    return ret;
}

Vector<Resource *> Resource_Cache::get_all(String package) const
{
    make_valid_package_name_(package);

    Vector<Resource *> ret;
    ret.reserve(resources_.size());
    auto iter = resources_.begin();
    while (iter != resources_.end())
    {
        if (iter->second->get_package() == package)
            ret.push_back(iter->second);
        ++iter;
    }
    return ret;
}

bool Resource_Cache::remove(u32 id)
{
    Resource * ret = nullptr;
    auto fiter = resources_.find(id);
    if (fiter != resources_.end())
    {
        ret = fiter->second;
        ret->terminate();
        fiter = resources_.erase(fiter);
        deallocate_resource_(ret, type_hash(ret->get_derived_info().m_type));
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
    else
        make_valid_package_name_(actual_package);

    if (actual_package.empty())
    {
        wlog("Cannot load resource {} - no package passed in and no current package set", name);
    }
    u32 id = str_hash(package + name);
    return remove(id);
}

bool Resource_Cache::remove(Resource * resource)
{
    return remove(resource->get_id());
}

void Resource_Cache::make_valid_package_name_(String & str) const
{
    if (str.find_last_of('/') != str.size() - 1)
    {
        str.push_back('/');
    }
}

bool Resource_Cache::load_package(String package, bool make_current)
{
    make_valid_package_name_(package);
    ilog("Trying to load package {}", package);
    auto inserted = loaded_packages_.emplace(package);
    if (inserted.second)
    {
        // Remove the final "/"
        String path_str = package.substr(0, package.size() - 1);
        if (fs::is_directory(path_str) || fs::create_directories(path_str))
        {
            ilog("Successfully created or found package dir {}", package);
        }
        else
        {
            wlog("Could not load package as package dir {} could not be created - is this a valid path?", package);
            loaded_packages_.erase(package);
            return false;
        }

        // Load all resources recursively in this directory!
        ilog("Trying to load all resources in {}", package);

        fs::recursive_directory_iterator dir_path_iter(path_str);
        while (dir_path_iter != fs::recursive_directory_iterator())
        {
            fs::path rel_path = dir_path_iter->path().relative_path();
            fs::path fname = rel_path.filename();
            // Skip hidden files and non regular files
            if (fname.empty() || fname.string()[0] == '.' || !fs::is_regular_file(*dir_path_iter))
            {
                ++dir_path_iter;
                continue;
            }

            String obj_name = rel_path.string();
            String ext = rel_path.extension().string();

            // Remove the extension from the file name, and the package prefix from the file name
            // This results in the obj name being what is left
            size_t np = obj_name.find(package);
            obj_name.erase(np, package.size());
            np = obj_name.find(ext);
            obj_name.erase(np, ext.size());
            u32 type_id = ns_ctxt.get_extension_resource_type(ext);

            if (type_id == INVALID_ID)
            {
                ilog("Skipping {} as the extension {} is not registered as a valid type",obj_name,ext);
                ++dir_path_iter;
                continue;
            }

            dlog("Loading obj name: {}", obj_name);
            dlog("Loading extension: {}", ext);
            dlog("Loading type id: {}", type_id);

            load(ns_ctxt.get_extension_resource_type(ext), obj_name, package);
            ++dir_path_iter;
        }

        if (make_current)
            set_current_package(package);

        return true;
    }

    wlog("Could not load package {} as it's already loaded", package);
    return false;
}

void Resource_Cache::save_all(String package) const
{
    make_valid_package_name_(package);
    ilog("Saving all resources in package {}!", package);
    auto iter = resources_.begin();
    while (iter != resources_.end())
    {
        if (iter->second->get_package() == package)
            iter->second->save();
        ++iter;
    }
}

void Resource_Cache::save_all() const
{
    ilog("Saving all resources!");
    auto iter = resources_.begin();
    while (iter != resources_.end())
    {
        iter->second->save();
        ++iter;
    }
}

void Resource_Cache::clear()
{
    while (resources_.begin() != resources_.end())
        remove(resources_.begin()->first);
}

void Resource_Cache::on_resource_name_change_(u32 old_id, u32 new_id, bool * do_change)
{
    auto fiter = resources_.find(old_id);
    if (fiter != resources_.end())
    {
        Resource * res = fiter->second;
        auto success = resources_.emplace(new_id, res);
        if (success.second)
        {
            resources_.erase(fiter);
        }
        else
        {
            *do_change = false;
            dlog("Name change denied by manager because name matches existing resource");
        }
    }
    else
    {
        elog("Couldn't find old id {} - this is a bug", old_id);
    }
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
        ilog("Set current package to {}", current_package_);
    }
    else
    {
        wlog("Cannot set package {} to current package - it's not loaded - load the package first!", package);
    }
}

Resource * Resource_Cache::allocate_resource_(u32 type_id)
{
    auto fac = ns_ctxt.get_factory(type_id);
    assert(fac != nullptr);
    Resource * res = fac->create_and_cast<Resource>();
    assert(res != nullptr);
    return res;
}

Resource * Resource_Cache::allocate_resource_(u32 type_id, const Resource & copy)
{
    auto fac = ns_ctxt.get_factory(type_id);
    assert(fac != nullptr);
    Resource * res = fac->create_and_cast<Resource>(copy);
    assert(res != nullptr);
    return res;
}

bool Resource_Cache::add_resource_(Resource * res, const String & name, const String & package, const Variant_Hash & init_params)
{
    String actual_package(package);
    if (actual_package.empty())
        actual_package = current_package_;
    else
        make_valid_package_name_(actual_package);

    if (actual_package.empty())
    {
        wlog("Cannot load resource {} - no package passed in and no current package set", name);
    }

    res->set_name(name);
    res->set_package(actual_package);
    auto fiter = resources_.emplace(res->get_id(), res);
    if (fiter.second)
    {
        sig_connect(res->change_id, this, &Resource_Cache::on_resource_name_change_);
        res->initialize(init_params);
        res->owned = true;
    }
    else
    {
        wlog("Could not add resource type {} with name {} to package {} as the resource id "
             "already exists",
             String(res->get_derived_info().m_type.get_name()),
             name,
             actual_package);
    }
    return fiter.second;
}

void Resource_Cache::deallocate_resource_(Resource * res, u32 type_id)
{
    assert(res != nullptr);
    auto fac = ns_ctxt.get_factory(type_id);
    fac->destroy(res);
}
} // namespace noble_steed
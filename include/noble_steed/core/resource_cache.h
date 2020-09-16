#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/signal.h>
#include <rttr/rttr_cast.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/core/resource.h>

namespace noble_steed
{
namespace init_param_key
{
namespace resource_cache
{
/// Vector<String> - A vector of directories which should be searched for package dirs
extern const String PACKAGE_DIRS;
/// String - The package dir name that should be set as the default package
extern const String CURRENT_PACKAGE;
} // namespace resource_cache
} // namespace init_param_key

class Resource_Cache
{
    friend class Context;

  public:
    void initialize(const Variant_Map & init_params);

    void terminate();

    // add resource
    template<class ResType>
    ResType * add(const String & name, const String & package = String(), const Variant_Map & init_params = Variant_Map())
    {
        rttr::type t = rttr::type::get<ResType>();
        Resource * res = add(t, name, package, init_params);
        if (res)
            return static_cast<ResType *>(res);
        return nullptr;
    }

    template<class ResType>
    ResType * add(const ResType & copy)
    {
        rttr::type t = rttr::type::get<ResType>();
        Resource * res = add_from_(copy);
        if (res)
            return static_cast<ResType *>(res);
        return nullptr;
    }

    Resource * add(const rttr::type & resource_type, const String & name, const String & package, const Variant_Map & init_params);

    Resource * add(u32 type_id, const String & name, const String & package, const Variant_Map & init_params);

    template<class ResType>
    ResType * get(u32 id)
    {
        Resource * res = get(id);
        if (res)
            return rttr::rttr_cast<ResType *>(res);
        return nullptr;
    }

    template<class ResType>
    ResType * get(const String & name, const String & package = String()) const
    {
        Resource * res = get(name, package);
        if (res)
            return rttr::rttr_cast<ResType *>(res);
        return nullptr;
    }

    Resource * get(const String & name, const String & package) const;

    Resource * get(u32 id) const;

    template<class ResType>
    Vector<ResType *> get_all() const
    {
        Vector<ResType *> ret;
        ret.reserve(resources_.size());
        auto iter = resources_.begin();
        while (iter != resources_.end())
        {
            ResType * cast = rttr::rttr_cast<ResType *>(iter->second);
            if (cast != nullptr)
                ret.push_back(cast);
            ++iter;
        }
        return ret;
    }

    template<class ResType>
    Vector<ResType *> get_all(String package) const
    {
        make_valid_package_name_(package);

        Vector<ResType *> ret;
        ret.reserve(resources_.size());
        auto iter = resources_.begin();
        while (iter != resources_.end())
        {
            if (iter->second->get_package() == package)
            {
                ResType * cast = rttr::rttr_cast<ResType *>(iter->second);
                if (cast != nullptr)
                    ret.push_back(cast);
            }
            ++iter;
        }
        return ret;
    }

    Vector<Resource *> get_all() const;

    Vector<Resource *> get_all(String package) const;

    template<class ResType>
    ResType * load(const String & name, const String & package = String(), const Variant_Map & init_params = Variant_Map())
    {
        rttr::type t = rttr::type::get<ResType>();
        Resource * res = load(t, name, package, init_params);
        if (res)
            return static_cast<ResType *>(res);
        return nullptr;
    }

    template<class ResType>
    ResType *
    load(const String & custom_path, const String & name, const String & package = String(), const Variant_Map & init_params = Variant_Map())
    {
        rttr::type t = rttr::type::get<ResType>();
        Resource * res = load(t, name, package, custom_path, init_params);
        if (res)
            return static_cast<ResType *>(res);
        return nullptr;
    }

    Resource *
    load(const rttr::type & resource_type, const String & name, const String & package = String(), const Variant_Map & init_params = Variant_Map());

    Resource * load(u32 type_id, const String & name, const String & package = String(), const Variant_Map & init_params = Variant_Map());

    Resource * load(const rttr::type & resource_type,
                    const String & custom_path,
                    const String & name,
                    const String & package = String(),
                    const Variant_Map & init_params = Variant_Map());

    bool remove(u32 id);

    bool remove(const String & name, const String & package = String());

    bool remove(Resource * resource);

    bool save(const String & custom_path, u32 resource_id) const;

    bool save(const String & custom_path, const String & res_name, const String & res_package) const;

    bool save(u32 resource_id) const;

    bool save(const String & res_name, const String & res_package) const;

    template<class ResType>
    void save_all() const
    {
        ilog("Saving all {} resources!", rttr::type::get<ResType>().get_name().to_string());
        auto iter = resources_.begin();
        while (iter != resources_.end())
        {
            ResType * cast = rttr::rttr_cast<ResType *>(iter->second);
            if (cast != nullptr)
                iter->second->save();
            ++iter;
        }
    }

    template<class ResType>
    void save_all(String package) const
    {
        make_valid_package_name_(package);
        ilog("Saving all {} resources in package {}!", rttr::type::get<ResType>().get_name().to_string(), package);
        auto iter = resources_.begin();
        while (iter != resources_.end())
        {
            if (iter->second->get_package() == package)
            {
                ResType * cast = rttr::rttr_cast<ResType *>(iter->second);
                if (cast != nullptr)
                    iter->second->save();
            }
            ++iter;
        }
    }

    void save_all() const;

    void save_all(String package) const;

    void clear();

    bool load_package(String package, bool make_current);

    void unload_package(String package);

    void set_current_package(String package);

  private:
    Resource_Cache();

    Resource_Cache(const Resource_Cache &) = delete;

    Resource_Cache & operator=(const Resource_Cache &) = delete;

    ~Resource_Cache();

    Resource * add_from_(const Resource & copy);

    void on_resource_name_change_(u32 old_id, u32 new_id, bool * do_change);

    Resource * allocate_resource_(u32 type_id);

    Resource * allocate_resource_(u32 type_id, const Resource & copy);

    bool add_resource_(Resource * res, const String & name, const String & package, const Variant_Map & init_params);

    void deallocate_resource_(Resource * res, u32 type_id);

    void make_valid_package_name_(String & str) const;

    Hash_Map<u32, Resource *> resources_;

    Hash_Set<String> loaded_packages_;

    String current_package_;

    SLOT_OBJECT
};

} // namespace noble_steed
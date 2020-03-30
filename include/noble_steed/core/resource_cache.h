#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/signal.h>

namespace noble_steed
{
class Resource;

extern const String INIT_PACKAGE_DIRS_KEY;
extern const String INIT_CURRENT_PACKAGE_KEY;

extern const String CORE_PACKAGE_NAME;
extern const String NONE_LOADED_PACKAGE_NAME;

class Resource_Cache
{
    SLOT_OBJECT
  public:
    Resource_Cache();

    ~Resource_Cache();

    // add resource
    template<class ResType>
    ResType * add(const String & name, const String & package = String(), const Variant_Map & init_params = Variant_Map())
    {
        rttr::type t = rttr::type::get<ResType>();
        return static_cast<ResType *>(add(t, name, package, init_params));
    }

    template<class ResType>
    ResType * add(const ResType & copy, const String & name, const String & package = String())
    {
        rttr::type t = rttr::type::get<ResType>();
        return static_cast<ResType *>(add_from(copy, name, package));
    }

    Resource * add(const rttr::type & resource_type, const String & name, const String & package, const Variant_Map & init_params);

    template<class ResType>
    ResType * get(u32 id)
    {
        return static_cast<ResType *>(get(id));
    }

    template<class ResType>
    ResType * get(const String & name, const String & package = String())
    {
        return static_cast<ResType *>(get(name, package));
    }

    Resource * get(const String & name, const String & package);

    Resource * get(u32 id);

    bool remove(u32 id);

    bool remove(const String & name, const String & package = String());

    bool remove(Resource * resource);

    void initialize(const Variant_Map & init_params);

    void terminate();

    void clear();

    bool load_package(String package, bool make_current);

    void unload_package(String package);

    void set_current_package(String package);

  private:
    Resource * add_from_(const Resource & copy, const String & name, const String & package);

    void on_resource_name_change_(u32 old_id, u32 new_id, bool * do_change);

    Resource * allocate_resource_(const rttr::type & type);

    Resource * allocate_resource_(const rttr::type & type, const Resource & copy);

    bool add_resource_(Resource * res, const String & name, const String & package, const Variant_Map & init_params);

    void deallocate_resource_(Resource * res, const rttr::type & resource_type);

    void make_valid_package_name_(String & str);

    Hash_Map<u32, Resource *> resources_;

    Hash_Set<String> loaded_packages_;

    String current_package_;
};

} // namespace noble_steed
#pragma once

#include <noble_steed/core/common.h>
#include <rttr/registration_friend>
#include <noble_steed/serialization/json_archive.h>
#include <noble_steed/core/context_obj.h>

namespace noble_steed
{
struct JSON_Archive;

class Resource : public Context_Obj
{
    friend class Resource_Cache;
  public:
    Resource();

    Resource(const Resource & copy);

    virtual ~Resource();

    virtual bool save();

    // Save data to custom file path - no regards to naming, packages, or extensions.
    virtual bool save(const String & custom_path);

    virtual bool load();

    // Load data from custom_path - no regards to naming, packages, or extensions. Custom path must be valid
    // file to succeed
    virtual bool load(const String & custom_path);

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

    const String & get_display_name();

    const String & get_name();

    const String & get_package();

    void set_display_name(const String & disp_name);

    void set_name(const String & name);

    void set_package(const String & package_name);

    String get_relative_path();

    u32 get_id();

    Signal<u32, u32, bool *> change_id;

    Resource & operator=(Resource rhs);

  protected:
    virtual void swap(Resource & rhs);

    String package_;
    String name_;
    String display_name_;
    u32 id_;

  private:
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(Context_Obj)
    JSON_PACKABLE
};
} // namespace noble_steed
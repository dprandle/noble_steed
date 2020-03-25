#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/signal.h>
#include <rttr/registration_friend>
#include <noble_steed/serialization/json_archive.h>


namespace noble_steed
{

struct JSON_Archive;

class Resource : public JSON_Packable
{
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(JSON_Packable)
    SLOT_OBJECT
  public:
    Resource();
    
    ~Resource();

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

    u64 get_id();

    Signal<u64, u64, bool*> change_id;

  private:
    String package_;
    String name_;
    String display_name_;
    u64 id_;
};
} // namespace noble_steed
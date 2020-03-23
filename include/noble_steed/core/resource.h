#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/signal.h>
#include <rttr/registration_friend>


namespace noble_steed
{

struct JSON_Archive;

class Resource
{
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE()
    SLOT_OBJECT
  public:
    Resource();
    ~Resource();

    virtual void save();

    virtual void load();

    virtual void pack_unpack(JSON_Archive & ar);

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

    Signal<u64, u64> change_id;

  private:
    String package_;
    String name_;
    String display_name_;
    u64 id_;
};
} // namespace noble_steed
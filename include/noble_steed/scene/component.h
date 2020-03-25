#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <rttr/registration_friend>
#include <noble_steed/serialization/json_archive.h>

namespace noble_steed
{
class Component: public JSON_Packable
{
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(JSON_Packable)
    SLOT_OBJECT
    friend class Entity;
  public:
    Component();
    virtual ~Component();

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

  private:
    u32 owner_id_;
};
} // namespace noble_steed

#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <rttr/registration_friend>

namespace noble_steed
{
class Component
{
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE()
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

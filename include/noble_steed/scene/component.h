#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <rttr/registration_friend>
#include <noble_steed/serialization/json_archive.h>
#include <noble_steed/core/context_obj.h>

namespace noble_steed
{
class Component : public Context_Obj
{
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(Context_Obj)
    JSON_PACKABLE
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

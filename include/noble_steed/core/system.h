#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <noble_steed/core/context_obj.h>

namespace noble_steed
{
class System : public Context_Obj
{
    RTTR_ENABLE(Context_Obj)
  public:
    System();
    virtual ~System();

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

  protected:
    SLOT_OBJECT
};

} // namespace noble_steed
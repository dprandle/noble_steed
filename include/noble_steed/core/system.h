#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>

namespace noble_steed
{
class System
{
    RTTR_ENABLE()
  public:
    System();
    virtual ~System();

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

  protected:
    SLOT_OBJECT
};

} // namespace noble_steed
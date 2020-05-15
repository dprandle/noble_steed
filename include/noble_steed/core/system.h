#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/context_obj.h>

namespace noble_steed
{
class System : public Context_Obj
{
    RTTR_ENABLE(Context_Obj)
  public:
    System();

    System(const System & copy);

    System & operator=(System rhs);

    virtual ~System();

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

  protected:
    void swap(const System & rhs);
};

} // namespace noble_steed
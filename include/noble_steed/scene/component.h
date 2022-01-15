#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/core/context_obj.h>

namespace noble_steed
{
class Component : public Context_Obj
{
    friend class Entity;

  public:
    Component();

    Component(const Component & copy);

    virtual ~Component();

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

    Component & operator=(Component rhs);

  protected:
    virtual void swap(Component & rhs);

  private:
    u32 owner_id_;
};
} // namespace noble_steed

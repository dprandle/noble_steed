#pragma once

#include <noble_steed/core/common.h>

namespace noble_steed
{
class Resource_Cache
{
  public:
    template<class T>
    void register_type()
    {
        //register_type_();
    }

  private:
    void register_type_(const rttr::type & t);
};

} // namespace noble_steed
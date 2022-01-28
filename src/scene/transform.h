#pragma once

#include "component.h"
#include "../math/vec4.h"

namespace noble_steed
{

class Transform : public Component
{
  public:
    Transform();
    ~Transform();

  private:
    vec4 position_;
    u16 dummy_;
};

} // namespace noble_steed
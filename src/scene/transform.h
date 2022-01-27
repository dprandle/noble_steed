#pragma once

#include "component.h"
#include "../math/vec4.h"

namespace noble_steed::scene
{

class Transform : public Component
{
  public:
    Transform();
    ~Transform();

  private:
    math::vec4 position_;
    u16 dummy_;
};

} // namespace noble_steed::scene
#pragma once

#include <noble_steed/scene/component.h>
#include <rttr/registration_friend>
#include <noble_steed/math/vec4.h>

namespace noble_steed
{
class Transform : public Component
{
    RTTR_REGISTRATION_FRIEND
    RTTR_ENABLE(Component)
  public:
    Transform();
    ~Transform();

  private:
    vec4 position_;
    u16 dummy_;
};
} // namespace noble_steed
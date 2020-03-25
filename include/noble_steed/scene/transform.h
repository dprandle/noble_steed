#pragma once

#include <noble_steed/scene/component.h>
#include <glm/vec4.hpp>
#include <rttr/registration_friend>

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
    glm::vec4 position_;
};
} // namespace noble_steed
#pragma once

#include <noble_steed/scene/component.h>

namespace noble_steed
{
class Transform : public Component
{
    RTTR_ENABLE(Component)

  public:
    Transform();
    ~Transform();

  private:
};
} // namespace noble_steed
#pragma once

#include <noble_steed/core/signal.h>
#include <rttr/type>
#include <glm/vec2.hpp>

namespace noble_steed
{
class Component
{
    RTTR_ENABLE()
    SLOT_OBJECT

  public:
    Component();
    virtual ~Component();

    virtual void initialize();

    virtual void terminate();

    void set_id(uint32_t id);

    uint32_t get_id();

    void set_owner_id(uint32_t owner_id);

    uint32_t get_owner_id();

    Signal<glm::uvec2> id_change;

    Signal<glm::uvec2> owner_id_change;

  private:
    uint32_t id_;
    uint32_t owner_id_;
};
} // namespace noble_steed
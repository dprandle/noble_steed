#pragma once

#include <noble_steed/core/common.h>

namespace noble_steed
{
class Component;

class Entity
{
  public:
    Entity();
    ~Entity();

    // add component
    template<class T>
    T * create_component(const T & copy = T())
    {
        
    }

    // remove component

    // create component

    // destroy component
  private:
    uint32_t id_;

};
} // namespace noble_steed
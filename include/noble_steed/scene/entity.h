#pragma once

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
};
} // namespace noble_steed
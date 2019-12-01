#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <noble_steed/container/hash_map.h>

namespace noble_steed
{
class Component;

class Entity
{
  public:
    Entity();
    ~Entity();

    // add component
    template<class T, class... Args>
    T * create_component(const Args &... args)
    {
        rttr::type t = rttr::type::get<T>();
        T * comp_ptr = static_cast<T *>(create_component_(t));
        new (comp_ptr) T(args...);

        comp_ptr->initialize();

        if (!add_component(comp_ptr))
        {
            destroy_component(comp_ptr);
            return nullptr;
        }
        return comp_ptr;
    }

    bool add_component(Component * comp);

    template<class T>
    T * get_component()
    {
        rttr::type t = rttr::type::get<T>();
        return static_cast<T*>(get_component(t));
    }

    Component * get_component(const rttr::type & type);

    template<class T>
    T * remove_component()
    {
        rttr::type t = rttr::type::get<T>();
        return static_cast<T*>(remove_component(t));
    }

    Component * remove_component(const rttr::type & type);

    template<class T>
    bool destroy_component()
    {
        rttr::type t = rttr::type::get<T>();
        return static_cast<T*>(destroy_component(t));
    }

    bool destroy_component(const rttr::type & type);

    bool destroy_component(Component * comp);

    void initialize();

    void terminate();

    void set_name(const String & name);

    String get_name();

    void set_id(u32 id);

    u32 get_id();

    // Set the id of the chunk that owns this entity
    void set_owner_id(sizet owner_id);

    // Get the id of the chunk that owns this entity
    sizet get_owner_id();

    // remove component

    // create component

    // destroy component

    // Emitted when the id is changed from a non-zero value to some different value
    Signal<Pair<u32>> id_change;

    // Emitted when the owner id is changed from a non-zero value to some different value
    Signal<Pair<sizet>> owner_id_change;

  private:
    Component * create_component_(const rttr::type & type);

    u32 id_;
    sizet owner_id_;
    String name_;
    Hash_Map<u64, Component *> comps_;
};
} // namespace noble_steed
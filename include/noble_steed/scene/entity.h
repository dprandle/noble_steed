#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <noble_steed/container/hash_map.h>

namespace noble_steed
{
class Component;
class World;
class Entity
{
  public:
    Entity();
    ~Entity();

    // add component
    template<class CompType>
    CompType * add(const Variant_Map & init_params = Variant_Map())
    {
        rttr::type t = rttr::type::get<CompType>();
        return static_cast<CompType *>(add(t));
    }

    template<class CompType>
    CompType * add(const CompType & copy)
    {
        rttr::type t = rttr::type::get<CompType>();
        return static_cast<CompType *>(add(t,copy));
    }

    Component * add(const rttr::type & component_type, const Variant_Map & init_params = Variant_Map());

    Component * add(const Component & copy);

    template<class CompType>
    CompType * get()
    {
        rttr::type t = rttr::type::get<CompType>();
        return static_cast<CompType*>(get(t));
    }

    Component * get(const rttr::type & component_type);

    template<class CompType>
    bool remove()
    {
        rttr::type t = rttr::type::get<CompType>();
        return remove(t);
    }

    bool remove(const rttr::type & component_type);

    bool remove(Component * component);

    void initialize();

    void terminate();

    void set_name(const String & name);

    String get_name();

    void set_id(u32 id);

    u32 get_id();

    Signal<Pair<u32>> id_change;

  private:
    Component * allocate_comp_(const rttr::type & type);
    
    Component * allocate_comp_(const rttr::type & type, const Component & copy);

    bool add_component_(Component * comp, const rttr::type & comp_type, const Variant_Map & init_params);

    void deallocate_comp_(Component * comp, const rttr::type & comp_type);
    
    Component * remove_component_(const rttr::type & type);
    
    u32 id_;
    
    String name_;
    
    Hash_Map<u64, Component *> comps_;
};
} // namespace noble_steed
#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <noble_steed/container/hash_map.h>
#include <noble_steed/serialization/json_archive.h>

namespace noble_steed
{
class Component;
class World;
class Entity: public JSON_Packable
{
    SLOT_OBJECT
    RTTR_ENABLE(JSON_Packable)
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

    void clear();

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

    void initialize(const Variant_Map & init_params);

    void terminate();

    void set_name(const String & name);

    const String & get_name();

    void set_id(u32 id);

    u32 get_id();

    void pack_unpack(JSON_Archive & ar);

    Signal<Pair<u32>> id_change;

  private:
    
    void pack_unpack_components(JSON_Archive & ar);

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
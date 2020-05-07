#pragma once

#include <noble_steed/core/signal.h>
#include <noble_steed/core/common.h>
#include <noble_steed/container/hash_map.h>
#include <noble_steed/serialization/json_archive.h>
#include <noble_steed/core/context_obj.h>
#include <rttr/registration_friend>

namespace noble_steed
{
class Component;
class World;
class Entity : public Context_Obj
{
    template<class T> friend class Pool_Factory;
    friend class World;

  public:
    Entity();

    Entity(const Entity & copy);

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
        return static_cast<CompType *>(add(t, copy));
    }

    Component * add(const rttr::type & component_type, const Variant_Map & init_params = Variant_Map());

    Component * add(const Component & copy);

    void clear();

    template<class CompType>
    CompType * get()
    {
        rttr::type t = rttr::type::get<CompType>();
        return static_cast<CompType *>(get(t));
    }

    Component * get(const rttr::type & component_type);

    template<class CompType>
    bool has()
    {
        rttr::type t = rttr::type::get<CompType>();
        return has(t);
    }

    bool has(const rttr::type & comp_type);

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

    Signal<Pair<u32>, bool *> id_change;

    Entity & operator=(Entity rhs);

  protected:
    virtual void swap(Entity & rhs);

    void pack_end(JSON_Archive::Direction io_dir);

  private:
    Component * allocate_comp_(const rttr::type & type);

    Component * allocate_comp_(const rttr::type & type, const Component & copy);

    bool add_component_(Component * comp, const rttr::type & comp_type, const Variant_Map & init_params);

    void deallocate_comp_(Component * comp, const rttr::type & comp_type);

    Component * remove_component_(const rttr::type & type);

    u32 id_;

    String name_;

    Hash_Map<u32, Component *> comps_;

    RTTR_REGISTRATION_FRIEND
    SLOT_OBJECT
    JSON_PACKABLE
    RTTR_ENABLE(Context_Obj)
};
} // namespace noble_steed
#pragma once

#include "../core/common.h"
#include "../core/context_obj.h"
#include "../core/tuple.h"
#include "../container/hash_map.h"
#include "../io/json_archive.h"
#include <rttr/registration_friend>

namespace noble_steed
{
class Component;
class World;
class Entity : public Context_Obj
{
    template<class T>
    friend class Pool_Factory;
    friend class World;

  public:
    Entity();

    Entity(const Entity & copy);

    ~Entity();

    // add component
    template<class CompType>
    CompType * add(const Variant_Hash & init_params = Variant_Hash())
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

    Component * add(const rttr::type & component_type, const Variant_Hash & init_params = Variant_Hash());

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

    void initialize(const Variant_Hash & init_params);

    void terminate();

    void set_name(const String & name);

    const String & get_name();

    void set_id(u32 id);

    u32 get_id();

    Signal<Tuple2<u32>, bool *> id_change;

    Entity & operator=(Entity rhs);

  protected:
    virtual void swap(Entity & rhs);

    void pack_begin(JSON_Archive::Direction io_dir);

    void pack_end(JSON_Archive::Direction io_dir);

  private:
    Component * allocate_comp_(const rttr::type & type);

    Component * allocate_comp_(const rttr::type & type, const Component & copy);

    bool add_component_(Component * comp, const rttr::type & comp_type, const Variant_Hash & init_params);

    void deallocate_comp_(Component * comp, const rttr::type & comp_type);

    Component * remove_component_(const rttr::type & type);

    u32 id_;

    String name_;

    Hash_Map<u32, Component *> comps_;

    RTTR_REGISTRATION_FRIEND
    JSON_PACKABLE
    RTTR_ENABLE(Context_Obj)
};

namespace events
{
namespace entity
{
/// This event is generated any time the \ref Entity.pack_begin function is called with direction in. This is to alert everyone that an entity
/// has been created with the entity pool allocator. World responds to this event and adds the entity to the "world".
namespace entity_packed_in
{
/// Hashed string id for the event
extern const u32 id;

/// Key for data to get the entity pointer - type is Entity *
extern const String entity_ptr;
} // namespace entity_packed_in

} // namespace entity
} // namespace events

} // namespace noble_steed
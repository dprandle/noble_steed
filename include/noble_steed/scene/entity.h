#pragma once

#include "../core/context_obj.h"

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
    CompType * add(const Variant_Map & init_params = Variant_Map())
    {
        type_index t = typeid(CompType);
        return static_cast<CompType *>(add(t));
    }

    template<class CompType>
    CompType * add(const CompType & copy)
    {
        type_index t = typeid(CompType);
        return static_cast<CompType *>(add(t, copy));
    }

    Component * add(const type_index & component_type, const Variant_Map & init_params = Variant_Map());

    Component * add(const Component & copy);

    void clear();

    template<class CompType>
    CompType * get()
    {
        type_index t = typeid(CompType);
        return static_cast<CompType *>(get(t));
    }

    Component * get(const type_index & component_type);

    template<class CompType>
    bool has()
    {
        type_index t = typeid(CompType);
        return has(t);
    }

    bool has(const type_index & comp_type);

    template<class CompType>
    bool remove()
    {
        type_index t = typeid(CompType);
        return remove(t);
    }

    bool remove(const type_index & component_type);

    bool remove(Component * component);

    void initialize(const Variant_Map & init_params);

    void terminate();

    void set_name(const String & name);

    const String & get_name();

    void set_id(u32 id);

    u32 get_id();

    Signal<Tuple2<u32>, bool *> id_change;

    Entity & operator=(Entity rhs);

  protected:
    virtual void swap(Entity & rhs);

  private:
    Component * allocate_comp_(const type_index & type);

    Component * allocate_comp_(const type_index & type, const Component & copy);

    bool add_component_(Component * comp, const type_index & comp_type, const Variant_Map & init_params);

    void deallocate_comp_(Component * comp, const type_index & comp_type);

    Component * remove_component_(const type_index & type);

    u32 id_;

    String name_;

    Hash_Map<type_index, Component *> comps_;
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
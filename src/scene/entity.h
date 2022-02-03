#pragma once

#include "../core/context_obj.h"
#include "../core/handle.h"
#include "../core/type_collection_db.h"

namespace noble_steed
{

struct Comp_Base
{
    virtual ~Comp_Base(){};
};

template<class CompT>

class Type_Collection_DB;
class World;
class Component;
class Entity : public Context_Obj
{
    template<class T>
    friend class Pool_Factory;
    friend class World;

  public:
    Entity();

    Entity(const Entity &copy);

    ~Entity();

    template<class T, class... Args>
    Handle<T> attach(Args &&...args)
    {}

    void clear();

    template<class CompType>
    CompType *get()
    {
        type_index t = typeid(CompType);
        return static_cast<CompType *>(get(t));
    }

    Component *get(const type_index &component_type);

    template<class CompType>
    bool has()
    {
        type_index t = typeid(CompType);
        return has(t);
    }

    bool has(const type_index &comp_type);

    template<class CompType>
    bool remove()
    {
        type_index t = typeid(CompType);
        return remove(t);
    }

    bool remove(const type_index &component_type);

    bool remove(Component *component);

    void initialize(const Variant_Map &init_params);

    void terminate();

    void set_name(const String &name);

    const String &get_name();

    void set_id(u32 id);

    u32 get_id();

    Signal<Tuple2<u32>, bool *> id_change;

    Entity &operator=(Entity rhs);

  protected:
    virtual void swap(Entity &rhs);

  private:
    Component *allocate_comp_(const type_index &type);

    Component *allocate_comp_(const type_index &type, const Component &copy);

    bool add_component_(Component *comp, const type_index &comp_type, const Variant_Map &init_params);

    void deallocate_comp_(Component *comp, const type_index &comp_type);

    Component *remove_component_(const type_index &type);

    Type_Hash_Map<Handle_Base *> _comps;

    u32 id_;

    String name_;

    hash_map<type_index, Component *> comps_;
};

} // namespace noble_steed
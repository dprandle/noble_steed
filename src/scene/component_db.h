#pragma once

#include "../container/hash_map.h"
#include "../container/vector.h"

struct Collection_Base
{}

template<class T>
struct Collection : public Collection_Base
{
    Vector<T> _data;
}

class Component_DB
{
    template<class CompType>
    bool register_component<CompType>(const String & type_display_name, int alloc_size)
    {
        Type_Info ti(type_display_name, sizeof(CompType));
        auto ins_iter = _comps_by_type.emplace(ti, _ll_allocator->allocate<Collection<CompType>>());
        if (fiter)
    }

    Hash_Map<Type_Info, Collection_Base *> _comps_by_type;
    Hash_Map<Type_Info, 
};
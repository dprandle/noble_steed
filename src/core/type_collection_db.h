#pragma once

#include "type_info.h"
#include "../container/hash_map.h"
#include "../container/vector.h"
#include "../memory/allocator.h"



namespace noble_steed
{
class Allocator;

struct Type_Collection_Base
{
    virtual ~Type_Collection_Base()
    {}
    //virtual Type_Collection_Base & operator=(const Type_Collection_Base & rhs) = 0;
};

template<class T>
class Type_Collection : public Type_Collection_Base
{
  public:
    ~Type_Collection()
    {}

    
    Vector<T> data;
    Type_Info type;
};

class Type_Collection_DB
{
    using Type_Collection_Ptr = UPtr<Type_Collection_Base, Alloc_Deleter>;
  public:
    Type_Collection_DB(Allocator * alloc=nullptr);
    ~Type_Collection_DB();

    Type_Collection_DB(const Type_Collection_DB &) = delete;
    Type_Collection_DB &operator=(Type_Collection_DB &) = delete;

    template<class T>
    Type_Collection<T> *create(sizet allocate_elements)
    {
        auto ti = type_id<T>();
        if (!ti)
            return nullptr;
        auto success_pair = _collections.emplace(ti, Type_Collection_Ptr{_alloc->allocate_and_construct<Type_Collection<T>>(), _alloc.get()});
        if (success_pair.second)
        {
            auto ptr = static_cast<Type_Collection<T> *>(success_pair.first->second.get());
            ptr->data.reserve(allocate_elements);
            ptr->type = ti;
            return ptr;
        }
        return nullptr;
    }

    template<class T>
    Type_Collection<T> *get()
    {
        auto ti = type_id<T>();
        auto fiter = _collections.find(ti);
        if (fiter != _collections.end())
            return static_cast<Type_Collection<T> *>(fiter->second.get());
        return nullptr;
    }

    void clear();

  private:
    SPtr<Allocator> _alloc;
    Type_Hash_Map<Type_Collection_Ptr> _collections;
};

} // namespace noble_steed
#pragma once

#include "string_hash.h"
#include "../container/hash_map.h"

namespace noble_steed
{
#define type_id_static(Type) Type_Info(#Type, sizeof(Type))


class Type_Info
{
  public:
    Type_Info();
    Type_Info(const String & str, sizet size);
    
    const String & name() const;

    sizet hash_code() const;

    sizet size() const;

    operator bool() const;

    bool operator==(const Type_Info & rhs) const;

  private:
    Str_Hash _cached_hash;
    String _name;
    sizet _size;
};

struct Hash_Type_Info_Functor
{
    sizet operator()(const Type_Info &ti) const noexcept
    {
        return ti.hash_code();
    }
};

template<class Val>
using Type_Hash_Map = hash_map<Type_Info, Val, Hash_Type_Info_Functor>;

template<class T>
inline Type_Info type_id() {return Type_Info();}

template<class T>
inline Type_Info type_id(const T &) {return Type_Info();}
} // namespace noble_steed

#define DECLARE_CUSTOM_TYPE(Type) namespace noble_steed \
{ \
template<> \
inline Type_Info type_id<Type>() {return noble_steed::type_id_static(Type);} \
template<> \
inline Type_Info type_id<Type>(const Type & val) {return noble_steed::type_id_static(Type);} \
}

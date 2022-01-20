#pragma once

#include "../container/string.h"
#include "../container/hash_map.h"
#include "basic_types.h"

namespace noble_steed
{

class Str_Hash
{
  public:
    Str_Hash();
    Str_Hash(const String &source_string);
    Str_Hash(const char * src_str);

    sizet value() const;

    #ifdef DEBUG_BUILD
    const String & get_source_str() const;
    #endif

    operator bool() const;

    bool operator==(const Str_Hash & rhs) const;

  private:
    sizet _val;
    #ifdef DEBUG_BUILD
    String _src;
    #endif
};

struct Hash_Str_Functor
{
    sizet operator()(const Str_Hash& strhash) const noexcept;
};

template<class Val>
using Str_Hash_Map = Hash_Map<Str_Hash, Val, Hash_Str_Functor>;

} // namespace noble_steed
#include <functional>
#include "string_hash.h"

namespace noble_steed
{

Str_Hash::Str_Hash() : _val(0)
{}

Str_Hash::Str_Hash(const String &source_string)
    : _val(std::hash<String>{}(source_string))
#ifdef DEBUG_BUILD
      ,
      _src(source_string)
#endif
{}

Str_Hash::Str_Hash(const char * src_str)
    : _val(std::hash<String>{}(src_str))
#ifdef DEBUG_BUILD
      ,
      _src(src_str)
#endif
{}

sizet Str_Hash::value() const
{
    return _val;
}

Str_Hash::operator bool() const
{
    return (_val != 0);
}

#ifdef DEBUG_BUILD
const String &Str_Hash::get_source_str() const
{
    return _src;
}
#endif

bool Str_Hash::operator==(const Str_Hash & rhs) const
{
    return _val == rhs._val;
}

sizet Hash_Str_Functor::operator()(const Str_Hash &strhash) const noexcept
{
    return strhash.value();
}

} // namespace noble_steed

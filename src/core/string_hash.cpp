#include <functional>
#include "string_hash.h"

namespace noble_steed
{

Str_Hash::Str_Hash(const String &source_string)
    : _val(std::hash<String>{}(source_string))
#ifdef DEBUG_BUILD
      ,
      _src(source_string)
#endif
{}

sizet Str_Hash::value()
{
    return _val;
}

#ifdef DEBUG_BUILD
const String & Str_Hash::get_source_str()
{
    return _src;
}
#endif
} // namespace noble_steed

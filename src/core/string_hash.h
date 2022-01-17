#pragma once

#include "../container/string.h"
#include "common.h"

namespace noble_steed
{

class Str_Hash
{
  public:
    Str_Hash(const String &source_string);

    sizet value();

    #ifdef DEBUG_BUILD
    const String & get_source_str();
    #endif

  private:
    sizet _val;
    #ifdef DEBUG_BUILD
    String _src;
    #endif
};

} // namespace noble_steed
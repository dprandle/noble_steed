#pragma once

#include <list>
#include "string.h"

namespace noble_steed
{
template<class T>
using List = std::list<T>;

using String_List = List<String>;
}
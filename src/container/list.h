#pragma once

#include <list>
#include "string.h"

namespace noble_steed
{
template<class T>
using list = std::list<T>;

using string_list = list<String>;
}
#pragma once

#include <map>

namespace noble_steed
{
template<class T, class S>
using Map = std::map<T,S>;

template<class T, class S>
using Multimap = std::multimap<T,S>;

}
#pragma once

#include <unordered_map>

namespace noble_steed
{
template<class T, class S>
using Hash_Map = std::unordered_map<T,S>;

template<class T, class S>
using Hash_Multimap = std::unordered_multimap<T,S>;

}
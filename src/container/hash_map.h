#pragma once

#include <unordered_map>

namespace noble_steed
{
template<class T, class S, class H=std::hash<T>>
using hash_map = std::unordered_map<T,S,H>;

template<class T, class S>
using hash_multimap = std::unordered_multimap<T,S>;

}
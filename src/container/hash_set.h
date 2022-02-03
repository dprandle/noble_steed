#pragma once

#include <unordered_set>

namespace noble_steed
{
template<class T>
using hash_set = std::unordered_set<T>;

template<class T>
using hash_multiset = std::unordered_set<T>;

}
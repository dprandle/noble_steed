#pragma once

#include <unordered_set>

namespace noble_steed
{
template<class T>
using Hash_Set = std::unordered_set<T>;

template<class T>
using Hash_Multiset = std::unordered_set<T>;

}
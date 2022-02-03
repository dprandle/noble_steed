#pragma once

#include "string.h"
#include <vector>

namespace noble_steed
{
template<class T>
using vector = std::vector<T>;
using i8_vector = vector<int8_t>;
using u8_vector = vector<uint8_t>;
using i16_vector = vector<int16_t>;
using u16_vector = vector<uint16_t>;
using i32_vector = vector<int32_t>;
using u32_vector = vector<uint32_t>;
using i64_vector = vector<int64_t>;
using u64_vector = vector<uint64_t>;
using string_vector = vector<String>;
}
#pragma once

#include "string.h"
#include <vector>

namespace noble_steed
{
template<class T>
using Vector = std::vector<T>;
using i8_Vector = Vector<int8_t>;
using u8_Vector = Vector<uint8_t>;
using i16_Vector = Vector<int16_t>;
using u16_Vector = Vector<uint16_t>;
using i32_Vector = Vector<int32_t>;
using u32_Vector = Vector<uint32_t>;
using i64_Vector = Vector<int64_t>;
using u64_Vector = Vector<uint64_t>;
using String_Vector = Vector<String>;
}
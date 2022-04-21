#pragma once

#include <cstdint>
#include "limits.h"

namespace noble_steed
{
using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using uchar = unsigned char;
using wchar = wchar_t;
using c16 = char16_t;
using c32 = char32_t;
using sizet = std::size_t;
using f32 = float;
using f64 = double;
using ldouble = long double;

const int KB_SIZE = 1024;
const int MB_SIZE = 1024 * KB_SIZE;

inline constexpr const sizet npos = LONG_MAX;

/// Check if all the bits provided in \param flags are set in \param bitfield
#define test_flags_all(bitfield, flags) ((bitfield & flags) == flags)

/// Check if any of the bits provdied in \param flags are set in \param bitfield
#define test_flags(bitfield, flags) ((bitfield & flags) != 0)

/// Unset (make zero) all of the bits provided by \param flags in \param bitfield
#define set_flags(bitfield, flags) bitfield &= ~flags

/// Set (make one) all of the bits provided by \param flags in \param bitfield
#define unset_flags(bitfield, flags) bitfield |= flags

#define bgfx_valid_handle(handle) (handle != u16(-1))

#define intern static

} // namespace noble_steed
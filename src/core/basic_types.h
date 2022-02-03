#pragma once

#include <cstdint>
#include <typeindex>
#include <memory>

namespace noble_steed
{
struct Invalid_Type
{};
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
using char16 = char16_t;
using char32 = char32_t;
using sizet = std::size_t;
using type_info = std::type_info;
using type_index = std::type_index;
using ldouble = long double;

#define INVALID_TYPE typeid(Invalid_Type)

const int KB_SIZE = 1024 * 8;
const int MB_SIZE = 1024 * KB_SIZE;

/// This is the default alloc amount - in terms of quantity of the components.. To set this to a different value
/// set the key CompName_Alloc with a u16 value indicating the quantity in init_params - where CompName is replaced with the type_info name
const u16 DEFAULT_COMP_ALLOC = 1000;

/// This is the default number of resources that will be allocated in each resource pool allocator
/// Set this to a different value by including ResType_Alloc as a key in init_params with the value as a u16
const u16 DEFAULT_RES_ALLOC = 100;

/// This is the pre-allocated amount for entities - adjust this as required
const u16 DEFAULT_ENTITY_ALLOC = 1000;

/// Check if all the bits provided in \param bitmask are set in \param flag
#define check_bitmask_all(flag, bitmask) ((flag & bitmask) == bitmask)

/// Check if any of the bits provdied in \param bitmask are set in \param flag
#define check_bitmask_any(flag, bitmask) ((flag & bitmask) != 0)

/// Unset (make zero) all of the bits provided by \param bitmask in \param flag
#define unset_bitmask(flag, bitmask) flag &= ~bitmask

/// Set (make one) all of the bits provided by \param bitmask in \param flag
#define set_bitmask(flag, bitmask) flag |= bitmask

#define bgfx_valid_handle(handle) (handle != u16(-1))

template<class T,class Deleter=std::default_delete<T>>
using UPtr = std::unique_ptr<T, Deleter>;
using std::make_unique;

template<class T>
using SPtr = std::shared_ptr<T>;
using std::make_shared;

template<class T>
using WPtr = std::weak_ptr<T>;

//MAKE_LOGGABLE_TEMPLATE(Tuple2);

} // namespace noble_steed
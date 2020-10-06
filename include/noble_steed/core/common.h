#pragma once

#include <rttr/type>
#include <utility>
#include <ostream>

#include "../container/string.h"
#include "../container/hash_map.h"
#include "../container/hash_set.h"
#include "../container/list.h"
#include "../container/vector.h"

std::ostream & operator<<(std::ostream & lhs, const rttr::variant & rhs);

namespace noble_steed
{

using Variant = rttr::variant;
using Variant_Map = Hash_Map<String, Variant>;
using Variant_List = List<Variant>;
using Variant_Vec = Vector<Variant>;

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

using i8_Vector = Vector<int8_t>;
using u8_Vector = Vector<uint8_t>;
using i16_Vector = Vector<int16_t>;
using u16_Vector = Vector<uint16_t>;
using i32_Vector = Vector<int32_t>;
using u32_Vector = Vector<uint32_t>;
using i64_Vector = Vector<int64_t>;
using u64_Vector = Vector<uint64_t>;


const int KB_SIZE = 1024 * 8;
const int MB_SIZE = 1024 * KB_SIZE;

/// This is the default alloc amount - in terms of quantity of the components.. To set this to a different value
/// set the key CompName_Alloc with a u16 value indicating the quantity in init_params - where CompName is replaced with the rttr registerd component name
const u16 DEFAULT_COMP_ALLOC = 1000;

/// This is the default number of resources that will be allocated in each resource pool allocator
/// Set this to a different value by including ResType_Alloc as a key in init_params with the value as a u16
const u16 DEFAULT_RES_ALLOC = 100;

/// This is the pre-allocated amount for entities - adjust this as required
const u16 DEFAULT_ENTITY_ALLOC = 1000;

/// Init params key to override the default entity allocation amount
const String ENTITY_ALLOC_KEY = "Entity_Alloc";

template<class T, class S>
using Pair = std::pair<T, S>;

/// Check if all the bits provided in \param bitmask are set in \param flag
#define check_bitmask_all(flag, bitmask) ((flag & bitmask) == bitmask)

/// Check if any of the bits provdied in \param bitmask are set in \param flag 
#define check_bitmask_any(flag, bitmask) ((flag & bitmask) != 0)

/// Unset (make zero) all of the bits provided by \param bitmask in \param flag
#define unset_bitmask(flag, bitmask) flag &= ~bitmask

/// Set (make one) all of the bits provided by \param bitmask in \param flag
#define set_bitmask(flag, bitmask) flag |= bitmask

#define bgfx_valid_handle(handle) (handle != u16(-1))

/// Simplified structure for holding two of anything (provided by \tparam T) with several different
/// alias accessors.
template<class T>
struct Tuple2
{
    Tuple2(T first_ = 0, T second_ = 0) : first(first_), second(second_)
    {}
    Tuple2(const std::pair<T, T> & pr) : first(pr.first), second(pr.second)
    {}

    union
    {
        T data[2];

        struct
        {
            T first;
            T second;
        };

        struct
        {
            T x;
            T y;
        };

        struct
        {
            T w;
            T h;
        };

        struct
        {
            T s;
            T t;
        };

        struct
        {
            T u;
            T v;
        };
    };

    Tuple2<T> operator-(const Tuple2<T> & rhs)
    {
        return Tuple2<T>(x - rhs.x, y - rhs.y);
    }

    Tuple2<T> operator-(const T & rhs)
    {
        return Tuple2<T>(x - rhs, y - rhs);
    }

    Tuple2<T> operator+(const Tuple2<T> & rhs)
    {
        return Tuple2<T>(x + rhs.x, y + rhs.y);
    }

    Tuple2<T> operator+(const T & rhs)
    {
        return Tuple2<T>(x + rhs, y + rhs);
    }

    Tuple2<T> & operator+=(const Tuple2<T> & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Tuple2<T> & operator+=(const T & rhs)
    {
        x += rhs;
        y += rhs;
        return *this;
    }

    Tuple2<T> & operator-=(const Tuple2<T> & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Tuple2<T> & operator-=(const T & rhs)
    {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    Tuple2<T> operator*(const T & rhs)
    {
        return Tuple2<T>(x * rhs, y * rhs);
    }

    Tuple2<T> operator/(const T & rhs)
    {
        return Tuple2<T>(x / rhs, y / rhs);
    }

    Tuple2<T> operator*=(const T & rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Tuple2<T> operator/=(const T & rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }
};

template<class T>
struct Tuple3
{
    Tuple3(T first_ = 0, T second_ = 0, T third_ = 0) : first(first_), second(second_), third(third_)
    {}
    union
    {
        T data[3];

        struct
        {
            T first;
            T second;
            T third;
        };

        struct
        {
            T x;
            T y;
            T z;
        };

        struct
        {
            T r;
            T g;
            T b;
        };

        struct
        {
            T s;
            T t;
            T p;
        };
    };

    Tuple3<T> operator-(const Tuple3<T> & rhs)
    {
        return Tuple3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Tuple3<T> operator-(const T & rhs)
    {
        return Tuple3<T>(x - rhs, y - rhs, z - rhs);
    }

    Tuple3<T> operator+(const Tuple3<T> & rhs)
    {
        return Tuple2<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Tuple3<T> operator+(const T & rhs)
    {
        return Tuple3<T>(x + rhs, y + rhs, z + rhs);
    }

    Tuple3<T> & operator+=(const Tuple3<T> & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Tuple3<T> & operator+=(const T & rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        return *this;
    }

    Tuple3<T> & operator-=(const Tuple3<T> & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Tuple3<T> & operator-=(const T & rhs)
    {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        return *this;
    }

    Tuple3<T> operator*(const T & rhs)
    {
        return Tuple3<T>(x * rhs, y * rhs, z * rhs);
    }

    Tuple3<T> operator/(const T & rhs)
    {
        return Tuple3<T>(x / rhs, y / rhs, z / rhs);
    }

    Tuple3<T> operator*=(const T & rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Tuple3<T> operator/=(const T & rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

};

template<class T>
struct Tuple4
{
    Tuple4(T first_ = 0, T second_ = 0, T third_ = 0, T fourth_ = 0) : first(first_), second(second_), third(third_), fourth(fourth_)
    {}
    union
    {
        T data[4];

        struct
        {
            T first;
            T second;
            T third;
            T fourth;
        };

        struct
        {
            T x;
            T y;
            T z;
            T w;
        };

        struct
        {
            T r;
            T g;
            T b;
            T a;
        };

        struct
        {
            T s;
            T t;
            T p;
            T q;
        };
    };

    Tuple4<T> operator-(const Tuple4<T> & rhs)
    {
        return Tuple4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Tuple4<T> operator-(const T & rhs)
    {
        return Tuple4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
    }

    Tuple4<T> operator+(const Tuple4<T> & rhs)
    {
        return Tuple2<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Tuple4<T> operator+(const T & rhs)
    {
        return Tuple4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
    }

    Tuple4<T> & operator+=(const Tuple4<T> & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Tuple4<T> & operator+=(const T & rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        w += rhs;
        return *this;
    }

    Tuple4<T> & operator-=(const Tuple4<T> & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Tuple4<T> & operator-=(const T & rhs)
    {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        w -= rhs;
        return *this;
    }

    Tuple4<T> operator*(const T & rhs)
    {
        return Tuple4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    Tuple4<T> operator/(const T & rhs)
    {
        return Tuple4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
    }

    Tuple4<T> operator*=(const T & rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    Tuple4<T> operator/=(const T & rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

};

template<class T>
bool grab_param(const Variant_Map & items, const String & name, T & to_fill)
{
    auto fiter = items.find(name);
    if (fiter != items.end() && fiter->second.is_type<T>())
    {
        to_fill = fiter->second.get_value<T>();
        return true;
    }
    return false;
}

using dtup2 = Tuple2<double>;
using ftup2 = Tuple2<float>;
using i8tup2 = Tuple2<i8>;
using i16tup2 = Tuple2<i16>;
using itup2 = Tuple2<i32>;
using i64tup2 = Tuple2<i64>;
using u8tup2 = Tuple2<u8>;
using u16tup2 = Tuple2<u16>;
using utup2 = Tuple2<u32>;
using u64tup2 = Tuple2<u64>;
using str_tup2 = Tuple2<String>;
using xtup2 = Tuple2<Variant>;

using dtup3 = Tuple3<double>;
using ftup3 = Tuple3<float>;
using i8tup3 = Tuple3<i8>;
using i16tup3 = Tuple3<i16>;
using itup3 = Tuple3<i32>;
using i64tup3 = Tuple3<i64>;
using u8tup3 = Tuple3<u8>;
using u16tup3 = Tuple3<u16>;
using utup3 = Tuple3<u32>;
using u64tup3 = Tuple3<u64>;
using str_tup3 = Tuple3<String>;
using xtup3 = Tuple3<Variant>;

using dtup4 = Tuple4<double>;
using ftup4 = Tuple4<float>;
using i8tup4 = Tuple4<i8>;
using i16tup4 = Tuple4<i16>;
using itup4 = Tuple4<i32>;
using i64tup4 = Tuple4<i64>;
using u8tup4 = Tuple4<u8>;
using u16tup4 = Tuple4<u16>;
using utup4 = Tuple4<u32>;
using u64tup4 = Tuple4<u64>;
using str_tup4 = Tuple4<String>;
using xtup4 = Tuple4<Variant>;

//MAKE_LOGGABLE_TEMPLATE(Tuple2);

} // namespace noble_steed
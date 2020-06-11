#pragma once

#include <noble_steed/container/string.h>
#include <noble_steed/container/hash_map.h>
#include <noble_steed/container/hash_set.h>
#include <noble_steed/container/list.h>
#include <rttr/type>
#include <utility>
#include <ostream>

std::ostream & operator<<(std::ostream & lhs, const rttr::variant & rhs);

namespace noble_steed
{
// #define MAKE_LOGGABLE(type)                                                                                                                          \
//     inline std::ostream & operator<<(std::ostream & lhs, const type & rhs)                                                                           \
//     {                                                                                                                                                \
//         rttr::instance i(rhs);                                                                                                                       \
//         return lhs << i;                                                                                                                             \
//     };

// #define MAKE_LOGGABLE_TEMPLATE(type)                                                                                                                 \
//     template<class T>                                                                                                                                \
//     std::ostream & operator<<(std::ostream & lhs, const type<T> & rhs)                                                                               \
//     {                                                                                                                                                \
//         rttr::instance i(rhs);                                                                                                                       \
//         return lhs << i;                                                                                                                             \
//     }

using Variant = rttr::variant;
using Variant_Map = Hash_Map<String, Variant>;
using Variant_List = List<Variant>;

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

const int KB_SIZE = 1024 * 8;
const int MB_SIZE = 1024 * KB_SIZE;

// This is the default alloc amount - in terms of quantity of the components.. To set this to a different value
// set the key CompName_Alloc with a u16 value indicating the quantity in init_params - where CompName is replaced with the rttr registerd component name
const u16 DEFAULT_COMP_ALLOC = 1000;

// This is the default number of resources that will be allocated in each resource pool allocator
// Set this to a different value by including ResType_Alloc as a key in init_params with the value as a u16
const u16 DEFAULT_RES_ALLOC = 100;

// This is the pre-allocated amount for entities - adjust this as required
const u16 DEFAULT_ENTITY_ALLOC = 1000;

// Init params key to override the default entity allocation amount
const String ENTITY_ALLOC_KEY = "Entity_Alloc";

template<class T, class S>
using Pair = std::pair<T, S>;

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
};

// template<class T>
// std::ostream & operator<<(std::ostream & lhs, const Tuple2<T> & rhs)
// {
//     return lhs << "[" << rhs.first << " " << rhs.second << "]";
// }

// template<class T>
// std::ostream & operator<<(std::ostream & lhs, const Tuple3<T> & rhs)
// {
//     return lhs << "[" << rhs.first << " " << rhs.second << " " << rhs.third << "]";
// }

// template<class T>
// std::ostream & operator<<(std::ostream & lhs, const Tuple4<T> & rhs)
// {
//     return lhs << "[" << rhs.first << " " << rhs.second << " " << rhs.third << " " << rhs.fourth << "]";
// }

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
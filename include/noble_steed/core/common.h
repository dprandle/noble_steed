#pragma once

#include <noble_steed/container/string.h>
#include <noble_steed/container/hash_map.h>
#include <rttr/type>
#include <utility>

namespace noble_steed
{
const int KB_SIZE = 1024 * 8;
const int MB_SIZE = 1024 * KB_SIZE;

using Variant_Map = Hash_Map<String, rttr::variant>;

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

template<class T>
struct Pair
{
    Pair(T first_=0, T second_=0):first(first_),second(second_){}
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
    Tuple3(T first_=0, T second_=0, T third_=0):first(first_),second(second_), third(third_){}
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
    Tuple4(T first_=0, T second_=0, T third_=0, T fourth_=0):first(first_),second(second_), third(third_), fourth(fourth_){}
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

} // namespace noble_steed
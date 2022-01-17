#pragma once

#include "string.h"
#include "pair.h"

namespace noble_steed
{

/// Simplified structure for holding two of anything (provided by \tparam T) with several different
/// alias accessors.
template<class T>
struct Tuple2
{
    Tuple2(T first_ = T(), T second_ = T()) : first(first_), second(second_)
    {}
    Tuple2(const Pair<T,T> & pr) : first(pr.first), second(pr.second)
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
    Tuple3(T first_ = T(), T second_ = T(), T third_ = T()) : first(first_), second(second_), third(third_)
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
    Tuple4(T first_ = T(), T second_ = T(), T third_ = T(), T fourth_ = T()) : first(first_), second(second_), third(third_), fourth(fourth_)
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

}
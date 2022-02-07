#pragma once

#include "nsvec3.h"

namespace noble_steed
{

template<class T>
struct cube
{
    cube<T>(const nsvec3<T> &min_ = nsvec3<T>(), const nsvec3<T> &max_ = nsvec3<T>()) : min(min_), max(max_)
    {}

    cube<T>(const cube<T> &copy) : min(copy.min), max(copy.max)
    {}

    cube<T> &operator=(const cube<T> &rhs)
    {
        if (this != &rhs)
            min = rhs.min;
        max = rhs.max;
        return *this;
    }

    union
    {
        struct
        {
            nsvec3<T> min;
            nsvec3<T> max;
        };

        struct
        {
            nsvec3<T> a;
            nsvec3<T> b;
        };
    };

    cube<T> operator+(const cube<T> &rhs)
    {
        return cube<T>(min + rhs.min, max + rhs.max);
    }

    cube<T> operator+(const nsvec3<T> &rhs)
    {
        return cube<T>(min + rhs, max + rhs);
    }

    cube<T> operator-(const cube<T> &rhs)
    {
        return cube<T>(min - rhs.min, max - rhs.max);
    }

    cube<T> operator-(const nsvec3<T> &rhs)
    {
        return cube<T>(min - rhs, max - rhs);
    }

    cube<T> &operator+=(const nsvec3<T> &rhs)
    {
        min += rhs;
        max += rhs;
        return *this;
    }

    cube<T> &operator-=(const nsvec3<T> &rhs)
    {
        min -= rhs;
        max -= rhs;
        return *this;
    }

    cube<T> &operator+=(const cube<T> &rhs)
    {
        min += rhs.min;
        max += rhs.max;
        return *this;
    }

    cube<T> &operator-=(const cube<T> &rhs)
    {
        min -= rhs.min;
        max -= rhs.max;
        return *this;
    }
};

using i8cube = cube<i8>;
using i16cube = cube<i16>;
using icube = cube<i32>;
using i64cube = cube<i64>;
using ui8cube = cube<u8>;
using ui16cube = cube<u16>;
using uicube = cube<u32>;
using ui64cube = cube<u64>;
using cube = cube<float>;
using dcube = cube<double>;
using ldcube = cube<ldouble>;

}
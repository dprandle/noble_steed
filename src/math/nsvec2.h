#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

#include "algorithm.h"

using namespace noble_steed;

template<class T>
struct nsmat2;

template<class T>
struct nsmat3;

template<class T>
struct nsvec2
{
    nsvec2(const T &init_ = 0);

    nsvec2(const T &x_, const T &y_);

    template<class U>
    nsvec2<U> convert() const
    {
        return {x, y};
    }

    nsvec2<T> minmax() const;

    T angle() const;

    T length() const;

    T length_sq() const;

    nsvec2<T> &set_length(const T &len_);

    nsvec2<T> &rotate(T angle_);

    nsvec2<T> &project(const nsvec2<T> &vec_);

    nsvec2<T> &project_plane(const nsvec2<T> &normal);

    nsvec2<T> &reflect(const nsvec2<T> &normal);

    nsvec2<T> &normalize();

    nsvec2<T> &abs();

    nsvec2<T> &ceil();

    nsvec2<T> &floor();

    nsvec2<T> &round(i8 decimal_places);

    nsvec2<T> &round();

    nsvec2<T> operator+(const nsvec2<T> &rhs_) const;

    nsvec2<T> operator-(const nsvec2<T> &rhs_) const;

    nsvec2<T> operator*(const nsvec2<T> &rhs_) const;

    nsvec2<T> operator/(const nsvec2<T> &rhs_) const;

    nsvec2<T> operator*(const T &rhs_) const;

    nsvec2<T> operator/(const T &rhs_) const;

    nsvec2<T> operator++(i32);

    nsvec2<T> operator--(i32);

    nsvec2<T> &operator++();

    nsvec2<T> &operator--();

    nsvec2<T> &operator+=(const nsvec2<T> &rhs_);

    nsvec2<T> &operator-=(const nsvec2<T> &rhs_);

    nsvec2<T> &operator*=(const nsvec2<T> &rhs_);

    nsvec2<T> &operator/=(const nsvec2<T> &rhs_);

    nsvec2<T> &operator*=(const T &rhs_);

    nsvec2<T> &operator/=(const T &rhs_);

    bool operator==(const nsvec2<T> &rhs_) const;

    bool operator<(const nsvec2<T> &rhs_) const;

    bool operator<=(const nsvec2<T> &rhs_) const;

    bool operator>(const nsvec2<T> &rhs_) const;

    bool operator>=(const nsvec2<T> &rhs_) const;

    bool operator!=(const nsvec2<T> &rhs_) const;

    bool operator==(const T &rhs_) const;

    bool operator!=(const T &rhs_) const;

    bool operator<(const T &rhs_) const;

    bool operator<=(const T &rhs_) const;

    bool operator>(const T &rhs_) const;

    bool operator>=(const T &rhs_) const;

    const T &operator[](sizet val_) const;

    T &operator[](sizet val_);

    static constexpr i8 size = 2;

    union
    {
        T data[size];

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
nsvec2<T> operator*(const T &lhs_, const nsvec2<T> &rhs_);

template<class T>
nsvec2<T> operator/(const T &lhs_, const nsvec2<T> &rhs_);

template<class T>
nsvec2<T> to_cartesian(const nsvec2<T> &polar);

template<class T>
nsvec2<T> to_polar(const nsvec2<T> &cartesian);

template<class T>
nsvec2<T> scaling_vec2(const nsmat2<T> &tform);

template<class T>
nsvec2<T> scaling_vec2(const nsmat3<T> &tform);

template<class T>
nsvec2<T> translation_vec2(const nsmat2<T> &tform);

template<class T>
nsvec2<T> translation_vec2(const nsmat3<T> &tform);

template<class T>
std::ostream &operator<<(std::ostream &os, const nsvec2<T> &vec);

// Math typedefs
using i8vec2 = nsvec2<i8>;
using i16vec2 = nsvec2<i16>;
using ivec2 = nsvec2<i32>;
using i64vec2 = nsvec2<i64>;
using u8vec2 = nsvec2<u8>;
using u16vec2 = nsvec2<u16>;
using uvec2 = nsvec2<u32>;
using u64vec2 = nsvec2<u64>;
using vec2 = nsvec2<float>;
using dvec2 = nsvec2<double>;
using ldvec2 = nsvec2<ldouble>;

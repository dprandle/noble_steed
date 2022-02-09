#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

#include "algorithm.h"

namespace noble_steed
{
template<class T>
struct nsmat2;

template<class T>
struct nsmat3;

template<class T>
struct vector2
{
    explicit vector2(const T &init_ = 0);

    vector2(const T &x_, const T &y_);

    template<class U>
    vector2<U> convert() const
    {
        return {x, y};
    }

    vector2<T> minmax() const;

    T angle() const;

    T length() const;

    T length_sq() const;

    vector2<T> &set_length(const T &len_);

    vector2<T> &rotate(T angle_);

    vector2<T> &project(const vector2<T> &vec_);

    vector2<T> &project_plane(const vector2<T> &normal);

    vector2<T> &reflect(const vector2<T> &normal);

    vector2<T> &normalize();

    vector2<T> &abs();

    vector2<T> &ceil();

    vector2<T> &floor();

    vector2<T> &round(i8 decimal_places);

    vector2<T> &round();

    vector2<T> operator+(const vector2<T> &rhs_) const;

    vector2<T> operator-(const vector2<T> &rhs_) const;

    vector2<T> operator*(const vector2<T> &rhs_) const;

    vector2<T> operator/(const vector2<T> &rhs_) const;

    vector2<T> operator*(const T &rhs_) const;

    vector2<T> operator/(const T &rhs_) const;

    vector2<T> operator++(i32);

    vector2<T> operator--(i32);

    vector2<T> &operator++();

    vector2<T> &operator--();

    vector2<T> &operator+=(const vector2<T> &rhs_);

    vector2<T> &operator-=(const vector2<T> &rhs_);

    vector2<T> &operator*=(const vector2<T> &rhs_);

    vector2<T> &operator/=(const vector2<T> &rhs_);

    vector2<T> &operator*=(const T &rhs_);

    vector2<T> &operator/=(const T &rhs_);

    bool operator==(const vector2<T> &rhs_) const;

    bool operator<(const vector2<T> &rhs_) const;

    bool operator<=(const vector2<T> &rhs_) const;

    bool operator>(const vector2<T> &rhs_) const;

    bool operator>=(const vector2<T> &rhs_) const;

    bool operator!=(const vector2<T> &rhs_) const;

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
vector2<T> operator*(const T &lhs_, const vector2<T> &rhs_);

template<class T>
vector2<T> operator/(const T &lhs_, const vector2<T> &rhs_);

template<class T>
std::ostream &operator<<(std::ostream &os, const vector2<T> &vec);

namespace math
{
template<class T>
noble_steed::vector2<T> to_cartesian(const noble_steed::vector2<T> &polar);

template<class T>
noble_steed::vector2<T> to_polar(const noble_steed::vector2<T> &cartesian);

template<class T>
noble_steed::vector2<T> scaling_vec2(const noble_steed::nsmat2<T> &tform);

template<class T>
noble_steed::vector2<T> scaling_vec2(const noble_steed::nsmat3<T> &tform);

template<class T>
noble_steed::vector2<T> translation_vec2(const noble_steed::nsmat2<T> &tform);

template<class T>
noble_steed::vector2<T> translation_vec2(const noble_steed::nsmat3<T> &tform);
} // namespace math

// Math typedefs
using i8vec2 = vector2<i8>;
using i16vec2 = vector2<i16>;
using ivec2 = vector2<i32>;
using i64vec2 = vector2<i64>;
using u8vec2 = vector2<u8>;
using u16vec2 = vector2<u16>;
using uvec2 = vector2<u32>;
using u64vec2 = vector2<u64>;
using vec2 = vector2<float>;
using dvec2 = vector2<double>;
using ldvec2 = vector2<ldouble>;
} // namespace noble_steed